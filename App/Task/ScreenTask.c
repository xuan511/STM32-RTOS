#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "OLED.h"
#include "sensor.h"
#include "main.h"
#include "app_state.h"

extern osMessageQueueId_t sensorQueueScreenHandle;

void StartScreenTask(void *argument)
{
    OLED_Init();
    SensorData_t data;
    uint8_t last_oled_state = 1; 
	if (g_ctrl.OLED_State == 1) 
    {
        OLED_Clear();
        OLED_ShowString(1, 1, "Brightness:");
        OLED_ShowString(2, 1, "Temp:");
        //OLED_ShowString(3, 1, "SE:   AE:  "); // 对应 g_ctrl 初始值
        //OLED_ShowString(4, 1, "[  INITIALIZING ]");
    }
    for(;;)
    {
		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, GPIO_PIN_SET);
        // 阻塞获取数据（数据源）
        if (osMessageQueueGet(sensorQueueScreenHandle, &data, NULL, osWaitForever) == osOK)
        {
            // 1：Key1 控制显示开关 ---
            if (g_ctrl.OLED_State == 0) 
            {
                if (last_oled_state == 1) {
                    OLED_Clear(); // 仅在关闭瞬间清屏一次，防止闪烁
                    last_oled_state = 0;
                }
                continue; // 彻底跳过后续所有显示代码
            }

            // 如果从关变开，重新刷初始化界面
            if (last_oled_state == 0) {
                OLED_Clear();
				OLED_ShowString(1, 1, "Brightness:");
				OLED_ShowString(2, 1, "Temp:");
                last_oled_state = 1;
            }

            // 2：正常显示数据 ---
            OLED_ShowNum(1, 12, data.brightness, 2);
			OLED_ShowString(1, 14, "%");
            OLED_ShowNum(2, 6, data.temp / 10, 2);
			OLED_ShowString(2, 8, ".");
            OLED_ShowNum(2, 9, data.temp % 10, 1);
			OLED_ShowString(2, 10, "C");

            // 3：Key2 报警状态 , 第三行显示 (0/1) ---
            OLED_ShowString(3, 1, "SE: "); 
            OLED_ShowNum(3, 4, g_ctrl.OLED_State, 1);   // 显示 0 或 1
            OLED_ShowString(3, 6, "AE:");
            OLED_ShowNum(3, 9, g_ctrl.AlarmEnable, 1); // 显示 0 或 1

            // 4：报警触发逻辑 ---
            if (g_ctrl.AlarmEnable && data.isTriggered) {
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
				//HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
                OLED_ShowString(4, 1, ">> ALARM ON <<");
            } else {
				
                OLED_ShowString(4, 1, "[  SYSTEM OK ]");
            }
        }
    }
}
