#ifndef DAC_OUTPUT_H
#define DAC_OUTPUT_H

#include "config.h"

class DACOutput {
  private:
    int basePin;
    int pinCount;

  public:
    // 构造函数
    DACOutput(int startPin, int numPins) : basePin(startPin), pinCount(numPins) {}
    
    // 初始化DAC引脚
    void begin() {
      for (int i = 0; i < pinCount; i++) {
        pinMode(basePin + i, OUTPUT);
        digitalWrite(basePin + i, LOW);
      }
    }
    
    // 输出8位数据到DAC
    void write(uint8_t value) {
      // 快速串行输出
      for (int i = 0; i < pinCount; i++) {
        digitalWrite(basePin + i, (value >> i) & 0x01);
      }
    }

};

#endif