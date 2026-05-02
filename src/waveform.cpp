#include "waveform.h"
#include <math.h>

// 构造函数
WaveformGenerator::WaveformGenerator() {
  phaseAccumulator = 0;
  lastTriangleValue = 0;
  triangleDirectionUp = true;
}

// 初始化
void WaveformGenerator::begin() {
  // 计算正弦表
  calculateSineTable();
  
  // 设置默认频率 (100Hz)
  setFrequency(100.0f);
  setAmplitude(1.0f); // 默认峰峰值为1.0 * 5  V= 5V
}

// 计算正弦波查找表
void WaveformGenerator::calculateSineTable() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    // 将0-2π映射到0-TABLE_SIZE
    float rad = (2.0f * PI * i) / TABLE_SIZE;
    
    // 正弦值范围[-1, 1] 映射到 [0, 255]
    // 公式: (sin(x) + 1) * 127.5
    float sineValue =  (sin(rad) + 1.0f) * 127.5f;
    
    // 限制范围并存储
    sineTable[i] = (uint8_t)constrain(sineValue, MIN_DAC_VALUE, MAX_DAC_VALUE);
  }
}
// 获取当前频率
float WaveformGenerator::getCurrentFrequency() {
  return 100.0f; // 目前固定为100Hz，可以根据需要扩展为动态频率
}

// 获取当前振幅
float WaveformGenerator::getCurrentAmplitude() {
  // 返回当前振幅设置
  return this->amplitude;
}
// 设置频率 (使用相位累加器)
void WaveformGenerator::setFrequency(float frequencyHz) {
  // 相位增量 = (期望频率 * 2^32) / 采样率
  // 使用32位相位累加器实现高精度频率控制
  float increment = (frequencyHz * powf(2.0f, 32)) / SAMPLE_RATE;
  phaseIncrement = (uint32_t)increment;
}

//设置振幅
void WaveformGenerator::setAmplitude(float amplitude) {
  
  this->amplitude = amplitude;
}

// 获取下一个正弦值
uint8_t WaveformGenerator::getNextSineValue() {
  // 从相位累加器的高位提取查找表索引
  uint32_t index = (phaseAccumulator >> 24) & 0xFF; // 取高8位作为索引
  phaseAccumulator += phaseIncrement;
  
  return (uint8_t)(amplitude * sineTable[index]);
}

// 获取下一个方波值
uint8_t WaveformGenerator::getNextSquareValue() {
  // 方波: 50%占空比
  uint32_t halfPeriod = 0x80000000; // 2^31
  
  phaseAccumulator += phaseIncrement;
  
  // 根据相位累加器的最高位决定输出高或低
  if (phaseAccumulator & 0x80000000) {
    return (uint8_t)(amplitude * MAX_DAC_VALUE);  // 高电平
  } else {
    return (uint8_t)(amplitude * MIN_DAC_VALUE);  // 低电平
  }
}

// 获取下一个三角波值
uint8_t WaveformGenerator::getNextTriangleValue() {
  // 三角波: 线性上升然后下降
  
  // 使用相位累加器的31位 (去掉符号位)
  uint32_t phase = phaseAccumulator & 0x7FFFFFFF;
  uint32_t quarter = 0x20000000; // 2^29
  uint32_t threeQuarter = 0x60000000; // 3 * 2^29
  
  phaseAccumulator += phaseIncrement;
  
  // 映射到三角波
  uint32_t value;
  
  if (phase < quarter) {
    // 上升阶段: 0 -> MAX (前1/4周期)
    value = map(phase, 0, quarter, MIN_DAC_VALUE, MAX_DAC_VALUE);
  } 
  else if (phase < threeQuarter) {
    // 下降阶段: MAX -> MIN (中间2/4周期)
    value = map(phase, quarter, threeQuarter, MAX_DAC_VALUE, MIN_DAC_VALUE);
  } 
  else {
    // 上升阶段: MIN -> MAX (最后1/4周期)
    value = map(phase, threeQuarter, 0x7FFFFFFF, MIN_DAC_VALUE, MAX_DAC_VALUE);
  }
  
  return (uint8_t)(value * amplitude);
}

// 根据波形类型获取下一个采样值
uint8_t WaveformGenerator::getNextSample(WaveformType waveform) {
  switch (waveform) {
    case WAVE_SQUARE:
      return getNextSquareValue();
    case WAVE_TRIANGLE:
      return getNextTriangleValue();
    case WAVE_SINE:
      return getNextSineValue();
    default:
      return 0;
  }
}

// 更新波形类型
WaveformType WaveformGenerator::updateWaveform(WaveformType currentWaveform) {
  WaveformType waveform = currentWaveform;
    switch (waveform) {
      case WAVE_SQUARE:
        waveform = WAVE_TRIANGLE;
        break;
      case WAVE_TRIANGLE:
        waveform = WAVE_SINE;
        break;
      case WAVE_SINE:
        waveform = WAVE_SQUARE;
        break;
      default:
        waveform = WAVE_SQUARE;
        break;
    }
  return waveform;
}

// 获取当前波形类型
WaveformType WaveformGenerator::getWaveformType() {
  return waveform;
}


