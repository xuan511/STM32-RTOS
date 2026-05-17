#include "AD.h"
#include "adc.h"
#include "FreeRTOSConfig.h"

// 定义全局缓冲区，由 DMA 自动填充
uint16_t AD_Buffer[CHANNEL_NUM * SAMPLE_DEPTH];

uint8_t AD_Init(void)
{
    HAL_StatusTypeDef ret;

    // ========== 1. ADC 校准 ==========
    // 校准有 10ms 超时，不会无限卡死
    ret = HAL_ADCEx_Calibration_Start(&hadc1);
    if (ret != HAL_OK)
    {
        // 校准失败：可能ADC时钟未开启、硬件未稳定、
        // 或校准过程中被更高优先级中断长时间抢占导致超时
        return AD_ERR_CAL;
    }

    // ========== 2. 启动 DMA 循环采样 ==========
    ret = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)AD_Buffer, CHANNEL_NUM * SAMPLE_DEPTH);
    if (ret != HAL_OK)
    {
        // DMA启动失败：可能DMA通道被占用或配置错误
        return AD_ERR_DMA;
    }

    return AD_OK;
}
