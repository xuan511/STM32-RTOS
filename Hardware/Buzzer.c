#include "cmsis_os.h" 
#include "stm32f1xx_hal.h"

/* 参数配置 */
#define ALARM_ARR_MAX  1000
#define ALARM_ARR_MIN  600

extern TIM_HandleTypeDef htim2;

/* 内部变量 */
static uint32_t currentARR = ALARM_ARR_MAX;
static int8_t step = -10;

/* 软件定时器 */
static osTimerId buzzerTimerHandle;

/* ================= 基础控制 ================= */

void Buzzer_Init(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); // 默认关闭
}

void Buzzer_On(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, currentARR / 2);
}

void Buzzer_Off(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
}

/* ================= 报警核心逻辑 ================= */

static void Buzzer_TimerCallback(void *argument)
{
    /* 动态变频 */
    currentARR += step;

    if (currentARR <= ALARM_ARR_MIN || currentARR >= ALARM_ARR_MAX)
    {
        step = -step;
    }

    __HAL_TIM_SET_AUTORELOAD(&htim2, currentARR);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, currentARR / 2);
}

/* ================= 对外接口 ================= */

void Buzzer_StartAlarm(void)
{
    currentARR = ALARM_ARR_MAX;
    step = -10;

    if (buzzerTimerHandle == NULL)
    {
        buzzerTimerHandle = osTimerNew(
            Buzzer_TimerCallback,
            osTimerPeriodic,
            NULL,
            NULL
        );
    }

    osTimerStart(buzzerTimerHandle, 10); // 10ms更新频率
}

void Buzzer_StopAlarm(void)
{
    if (buzzerTimerHandle != NULL)
    {
        osTimerStop(buzzerTimerHandle);
    }

    Buzzer_Off();
}
