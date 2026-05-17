#include "cmsis_os.h"
#include "sensor.h"
#include "stdio.h"
#include "serial.h"
//extern void Serial_SendString(char *str);
extern osMessageQueueId_t sensorQueueCommHandle;


/* StartCommTask.c */
void StartCommTask(void *argument)
{
	Serial_Init();
    SensorData_t data;
    char buf[48];
	
    for (;;)
    {
		osStatus_t status = osMessageQueueGet(sensorQueueCommHandle, &data, NULL, osWaitForever); 
		if (status == osOK) {
			// 正常处理...
			int len = sprintf(buf, "B:%d%% T:%d.%d Trigger:%d\r\n", 
				 data.brightness, data.temp/10, data.temp%10, data.isTriggered);
//						// VOFA+解析通道
//			int len = sprintf(buf, "%d %d.%d %d\n", 
//                 data.brightness, 
//                 data.temp / 10, 
//                 data.temp % 10, 
//                 data.isTriggered);
			HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 100);
			osDelay(20);
		} else {
			// 如果获取失败（例如超时），打印错误信息
			Serial_SendString("Queue get timeout!\r\n");
		}
		

    }
}
