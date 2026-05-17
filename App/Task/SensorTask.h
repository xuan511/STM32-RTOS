#ifndef __SENSORTASK_H
#define __SENSORTASK_H

#include "stdint.h"

typedef struct
{
    uint16_t brightness;
    int16_t temp_x10;
    uint8_t isTriggered;
} SensorData_t;

#endif