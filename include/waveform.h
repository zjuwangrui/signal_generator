#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "config.h"

class WaveformGenerator {
  private:
    WaveformType waveform;
    // 正弦波查找表
    uint8_t sineTable[TABLE_SIZE];
    
    // 当前相位累加器 (用于生成连续波形)
    uint32_t phaseAccumulator;
    uint32_t phaseIncrement;
    
    // 上一次输出的值 (用于三角波方向控制)
    uint8_t lastTriangleValue;
    bool triangleDirectionUp;
    
    // 内部方法
    void calculateSineTable();
    uint8_t getNextSineValue();
    uint8_t getNextSquareValue();
    uint8_t getNextTriangleValue();

  public:
    // 构造函数
    WaveformGenerator();
    
    // 初始化波形生成器
    void begin();
    
    // 设置频率 (基于相位累加器)
    void setFrequency(float frequencyHz);
    void setAmplitude(uint8_t amplitude);
    // 根据波形类型获取下一个采样值
    uint8_t getNextSample(WaveformType waveform);
    
    // 读取波形选择引脚，返回波形类型
    WaveformType updateWaveform(WaveformType currentWaveform);
    WaveformType getWaveformType();
    
    // 获取当前频率设置
    float getCurrentFrequency();
    // 获取当前振幅设置
    uint8_t getCurrentAmplitude();
};

#endif