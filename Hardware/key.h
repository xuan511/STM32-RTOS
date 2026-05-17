#ifndef __KEY_H
#define __KEY_H

#include "main.h"

// 按键结构体：记录每个按键的私有状态
typedef struct
{
    uint8_t lastState;     // 上次读取的物理电平
    uint8_t stableState;   // 消除抖动后的稳定电平
    uint32_t lastTick;     // 上次电平发生跳变的时间戳
} Key_t;

// 函数接口
void Key_Init(void);
void Key_Process(void);
uint8_t Key1_Pressed(void);
uint8_t Key2_Pressed(void);

#endif
