#ifndef __SERIAL_H
#define __SERIAL_H

#include "usart.h"
#include <stdint.h>
void Serial_Init(void);
void Serial_SendString(char *str);
void Serial_SendArray(uint8_t *data, uint16_t len);
void Serial_SendPacket(uint8_t bright, uint16_t temp, uint8_t status);
void Serial_SendPacket_CRC(uint8_t bright, uint16_t temp, uint8_t status);

#endif
