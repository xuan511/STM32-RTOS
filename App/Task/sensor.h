#ifndef __SENSOR_H
#define __SENSOR_H

typedef struct
{
    uint16_t brightness;
    uint16_t temp;
    uint8_t isTriggered;
} SensorData_t;

#endif
