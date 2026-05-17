#include "key.h"
#include "cmsis_os.h"

// 内部私有变量
static Key_t key1 = {1, 1, 0}; // 假设上拉电阻，初始为高电平
static Key_t key2 = {1, 1, 0};

static uint8_t key1_event = 0;
static uint8_t key2_event = 0;

void Key_Init(void)
{
    // GPIO初始化已在 main.c 的 MX_GPIO_Init 中完成
    // 确保 CubeMX 中 Key1/Key2 设为 GPIO_Input 且 Pull-up
}

/**
 * @brief 按键扫描核心逻辑（放入 InputTask 中每 10-20ms 调用一次）
 */
void Key_Process(void)
{
    uint32_t now = osKernelGetTickCount();

    // ----- KEY1 处理 -----
    uint8_t val1 = HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin);
    if (val1 != key1.lastState) {
        key1.lastState = val1;
        key1.lastTick = now; // 记录跳变时刻
    } else {
        if ((now - key1.lastTick) > 20) { // 持续稳定 20ms
            if (val1 != key1.stableState) {
                key1.stableState = val1;
                // 检测下降沿（从高变低代表按下）
                if (val1 == GPIO_PIN_RESET) key1_event = 1;
            }
        }
    }

    // ----- KEY2 处理 -----
    uint8_t val2 = HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin);
    if (val2 != key2.lastState) {
        key2.lastState = val2;
        key2.lastTick = now;
    } else {
        if ((now - key2.lastTick) > 20) {
            if (val2 != key2.stableState) {
                key2.stableState = val2;
                if (val2 == GPIO_PIN_RESET) key2_event = 1;
            }
        }
    }
}

/**
 * @brief 暴露给应用层的事件获取函数（读取后自动清零）
 */
uint8_t Key1_Pressed(void) {
    if (key1_event) {
        key1_event = 0;
        return 1;
    }
    return 0;
}

uint8_t Key2_Pressed(void) {
    if (key2_event) {
        key2_event = 0;
        return 1;
    }
    return 0;
}
