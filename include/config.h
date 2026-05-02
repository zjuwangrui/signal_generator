#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============ 引脚定义 ============
// DAC数据引脚 (假设使用并行8位DAC，如DAC0808)
#define DAC_BASE_PIN     0      // DAC数据起始引脚 (D0-D7共8位)
#define DAC_DATA_PINS    8      // 数据线数量
#define SQUARE_PIN   11     // 方波输出引脚
// 波形选择引脚
#define WAVE_SELECT_PIN  10     // D10数字输入读取波形选择

// 幅度控制引脚
#define AMP_POT_PIN      A0     // 电位器调节幅度

// ============ 波形参数 ============
#define SAMPLE_RATE      1000   // 采样率(Hz) - 每秒钟输出的点数
#define TABLE_SIZE       256    // 正弦波查找表大小
#define MAX_DAC_VALUE    255    // 8位DAC最大值
#define MIN_DAC_VALUE    0      // 8位DAC最小值
#define SQUARE_T         20  // 方波周期 (毫秒)

// ============ 波形类型定义 ============
enum WaveformType {
  WAVE_SQUARE = 0,    // 方波
  WAVE_TRIANGLE = 1,  // 三角波
  WAVE_SINE = 2,      // 正弦波
};

#endif