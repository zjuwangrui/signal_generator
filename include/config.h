#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============ 引脚定义 ============
// DAC数据引脚 (假设使用并行8位DAC，如DAC0808)
#define DAC_BASE_PIN     2      // DAC数据起始引脚 (D2-D9共8位)
#define DAC_DATA_PINS    8      // 数据线数量

// 波形选择引脚
#define WAVE_SELECT_PIN  1     // 数字输入读取波形选择

// 幅度控制引脚
#define AMP_POT_PIN      A1     // 电位器调节幅度

// ============ 波形参数 ============
#define SAMPLE_RATE      1000   // 采样率(Hz) - 每秒钟输出的点数
#define TABLE_SIZE       256    // 正弦波查找表大小
#define MAX_DAC_VALUE    255    // 8位DAC最大值
#define MIN_DAC_VALUE    0      // 8位DAC最小值

// ============ 波形类型定义 ============
enum WaveformType {
  WAVE_SQUARE = 0,    // 方波
  WAVE_TRIANGLE = 1,  // 三角波
  WAVE_SINE = 2,      // 正弦波
  WAVE_TYPE_COUNT = 3 // 波形类型总数
};

#endif