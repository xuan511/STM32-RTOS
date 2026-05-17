#ifndef __APP_STATE_H
#define __APP_STATE_H

#include <stdint.h>

typedef struct
{
    volatile uint8_t OLED_State;
    volatile uint8_t AlarmEnable;
    volatile uint8_t ShowMode;
} ControlData_t;

extern ControlData_t g_ctrl;

#endif
