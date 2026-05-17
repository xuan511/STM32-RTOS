#include "cmsis_os.h"
#include "key.h"
#include "app_state.h"
void StartInputTask(void *argument)
{
    for(;;)
    {
        Key_Process();

        // Key1：切换 OLED 显示/关闭状态
        if (Key1_Pressed())
        {
            g_ctrl.OLED_State = !g_ctrl.OLED_State;
        }

        // Key2：切换报警使能
        if (Key2_Pressed())
        {
            g_ctrl.AlarmEnable = !g_ctrl.AlarmEnable;
        }

        osDelay(20); // 稍微加大延时，按键扫描更稳
    }
}
