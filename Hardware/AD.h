#ifndef __AD_H
#define __AD_H

#include "main.h"
#include <stdint.h>

#define SAMPLE_DEPTH 10  // 每个通道一次采 10 组数据做平均
#define CHANNEL_NUM  2   // 光敏 + NTC

// AD_Init 返回值定义
#define AD_OK      0
#define AD_ERR_CAL 1   // 校准失败
#define AD_ERR_DMA 2   // DMA启动失败

uint8_t AD_Init(void);

#endif
