#include "cmsis_os.h"
#include "AD.h"
#include "senor.h"

extern osMessageQueueId_t sensorQueueScreenHandle;
extern osMessageQueueId_t sensorQueueCommHandle;
extern uint16_t AD_Buffer[CHANNEL_NUM * SAMPLE_DEPTH];

static uint16_t Calculate_Temp(uint16_t adc_val) {
    // 保护：防止溢出或除零
    if (adc_val < 500) return 600;  // 超过 60℃
    if (adc_val > 3800) return 50;  // 低于 5℃

    // 基于 10K 上拉电阻和 B=3950 的 NTC 的二次曲线拟合
    float x = (float)adc_val;
    float temp_f = 0.0000035f*x*x - 0.038f*x + 88.5f; 
    
    return (uint16_t)(temp_f * 10);
}
void StartSensorTask(void *argument)
{
    SensorData_t data;

    // ADC初始化（校准 + 启动DMA）
    // 如果失败，会导致传感器数据异常，但不应阻塞系统
    if (AD_Init() != AD_OK)
    {
        // 初始化失败时填充默认安全值，防止下游使用未初始化数据
        data.brightness = 50;  // 默认中等亮度
        data.temp = 250;       // 默认 25.0℃
        data.isTriggered = 0;
        osMessageQueuePut(sensorQueueScreenHandle, &data, 0, 0);
    }

    for (;;)
    {
        uint32_t sum_l = 0, sum_t = 0;

        // 1. 均值滤波：通过 SAMPLE_DEPTH 提升精度
        for (int i = 0; i < SAMPLE_DEPTH * CHANNEL_NUM; i += 2) {
            sum_l += AD_Buffer[i];     
            sum_t += AD_Buffer[i + 1]; 
        }
        uint16_t avg_l = sum_l / SAMPLE_DEPTH;
        uint16_t avg_t = sum_t / SAMPLE_DEPTH;

        // 2. 亮度计算 (0-4095 反向映射到 0-100)
        data.brightness = 100 - (avg_l * 100 / 4095);
        
        // 3. 温度计算 (带入 10K 电阻参数的拟合算法)
        data.temp = Calculate_Temp(avg_t); 

        // 4. 判定逻辑
        data.isTriggered = (data.brightness < 45 || data.temp > 290) ? 1 : 0;

        // 5. 极速响应：通过队列发送
        osMessageQueuePut(sensorQueueScreenHandle, &data, 0, 0);
		osMessageQueuePut(sensorQueueCommHandle, &data, 0, 0);
        // 实时性设置
        osDelay(500); 
    }
}
// NTC 计算函数
//static float NTC_ToTemp(float R) {
//    float B = 3950.0f, R0 = 10000.0f, T0 = 298.15f;
//    if (R <= 0) return 0;
//    float temp = (1.0f / ((1.0f / B) * logf(R / R0) + (1.0f / T0))) - 273.15f;
//    return temp;
//}
//void StartSensorTask(void *argument)
//{
//    SensorData_t data;
//    AD_Init(); // 任务开始前校准一次

//    for (;;)
//    {
//        // 1. 手动触发一次 DMA 采样（双通道）
//        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)AD_Raw, 2);
//        
//        // 2. 等待采样完成（10ms 足够 ADC 完成并让 CPU 歇会儿）
//        osDelay(10); 
//        
//        // 3. 读取并处理数据
//        uint16_t adc_light = AD_Raw[0];
//        uint16_t adc_ntc   = AD_Raw[1];

//        // 保护：防止除以 0 导致死机
//        if (adc_ntc >= 4095) adc_ntc = 4094;
//        
//        /* 计算光照 */
//        data.brightness = 100 - (adc_light / 41); // 0-4095 映射到 0-100

//        /* 计算温度 */
//        float R_fixed = 10000.0f;
//        float R_ntc = R_fixed * adc_ntc / (4095.0f - adc_ntc);
//        float temp_val = NTC_ToTemp(R_ntc);
//        data.temp = (uint16_t)(temp_val * 10); // 存放大 10 倍的整数

//        /* 判定逻辑 */
//        data.isTriggered = (data.brightness < 30 || temp_val > 35.0f) ? 1 : 0;

//        /* 广播：发送到各个队列 */
//        osMessageQueuePut(sensorQueueScreenHandle, &data, 0, 0);
//        osMessageQueuePut(sensorQueueCommHandle, &data, 0, 0);

//        // 4. 控制整体采样率：每 200ms 更新一次数据，防止屏幕狂闪
//        osDelay(200); 
//    }
//}
