#include "cmsis_os.h"
#include "senor.h"
#include "stdio.h"
#include "serial.h"
extern void Serial_SendString(char *str);
extern osMessageQueueId_t sensorQueueCommHandle;


/* StartCommTask.c */
void StartCommTask(void *argument)
{
    SensorData_t data;
    char buf[48];

    for (;;)
    {
        // 阻塞等待数据
        if (osMessageQueueGet(sensorQueueCommHandle, &data, NULL, osWaitForever) == osOK)
        {
            // 拼装字符串
            int len = sprintf(buf, "B:%d%% T:%d.%d Trigger:%d\r\n", 
                             data.brightness, data.temp/10, data.temp%10, data.isTriggered);
            
            // 使用 HAL 库发送
            HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 100);
        }
    }
}
