#include "stm32f1xx_hal.h"
#include <string.h>
#include "app_state.h"
extern UART_HandleTypeDef huart1;
//uint8_t uart1_rx_byte;   // HAL专用接收缓存
uint8_t RxData; // 状态机使用变量
uint8_t OLED_State = 1;
void Serial_Init(void)
{
    g_ctrl.OLED_State = 1;
    HAL_UART_Receive_IT(&huart1, &RxData, 1);
}

void Serial_SendPacket(uint8_t bright, uint16_t temp, uint8_t status)
{
    uint8_t buf[6];

    buf[0] = 0xAA;
    buf[1] = bright;
    buf[2] = (temp >> 8) & 0xFF;
    buf[3] = temp & 0xFF;
    buf[4] = status;
    buf[5] = 0x55;

    HAL_UART_Transmit(&huart1, buf, 6, HAL_MAX_DELAY);
}
void Serial_SendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

void Serial_SendArray(uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, HAL_MAX_DELAY);
}

void Serial_SendString(char *str)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

//void USART1_IRQHandler(void)
//{
//    HAL_UART_IRQHandler(&huart1);
//}

//回调函数
//HAL_UART_IRQHandler → Callback函数 → 自动恢复接收
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t RxState = 0;

    if (huart->Instance == USART1)
    {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
		// AA 00/01 FF 0D 0A
        if (RxState == 0 && RxData == 0xAA) RxState = 1;
        else if (RxState == 1)
        {
            g_ctrl.OLED_State = RxData;//这一位0或1，参数传到OLED_State
            RxState = 2;
        }
        else if (RxState == 2 && RxData == 0xFF) RxState = 3;
        else if (RxState == 3 && RxData == 0x0D) RxState = 4;
        else if (RxState == 4 && RxData == 0x0A) RxState = 0;
        else RxState = 0;

        // 关键：重新开启接收
        HAL_UART_Receive_IT(&huart1, &RxData, 1);
    }
}

//void USART1_IRQHandler(void)
//{
//    static uint8_t RxState = 0;

//    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {
//        RxData = USART_ReceiveData(USART1);

//        if (RxState == 0 && RxData == 0xAA) RxState = 1;
//        else if (RxState == 1)
//        {
//            OLED_State = RxData;
//            RxState = 2;
//        }
//        else if (RxState == 2 && RxData == 0xFF) RxState = 3;
//        else if (RxState == 3 && RxData == 0x0D) RxState = 4;
//        else if (RxState == 4 && RxData == 0x0A) RxState = 0;
//        else RxState = 0;

//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//    }
//}
