#include "AD.h"
#include "adc.h"
#include "FreeRTOSConfig.h"

// 定义全局缓冲区，由 DMA 自动填充
uint16_t AD_Buffer[CHANNEL_NUM * SAMPLE_DEPTH];

/**
 * @brief  初始化ADC+DMA（包含校准）
 * @retval AD_OK（0）成功 / AD_ERR_CAL（1）校准失败 / AD_ERR_DMA（2）DMA启动失败
 * 
 * @note   重要！⚠️ 此函数只能在任务启动后调用一次！
 *         原因：连续模式 + CIRCULAR DMA 启动后，
 *         DMA 状态永不为 READY，重复调用会返回 HAL_BUSY。
 *         ADC 的校准函数内部有 10ms 超时，不会永久卡死。
 */
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
    // 注意：连续模式 + CIRCULAR DMA 启动后，
    // DMA 状态会被永久置为 BUSY（直到调用 Stop_DMA）
    // 这就是为什么 HAL_ADC_Start_DMA 只能调用一次！
    ret = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)AD_Buffer, CHANNEL_NUM * SAMPLE_DEPTH);
    if (ret != HAL_OK)
    {
        // DMA启动失败：可能DMA通道被占用或配置错误
        return AD_ERR_DMA;
    }

    return AD_OK;
}
