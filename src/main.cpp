/**
 * Arduino 函数波形发生器
 * 
 * 硬件连接:
 * - DAC数据: D2-D9 (8位并行数据)
 * - 波形选择: D1 (按钮)
 * - 幅度调节: A1 (电位器)
 * 
 * 输出波形: 方波、三角波、正弦波
 */

#include "config.h"
#include "waveform.h"
#include "dac_output.h"

// ============ 全局对象 ============
WaveformGenerator waveformGen;
DACOutput dacOutput(DAC_BASE_PIN, DAC_DATA_PINS);

// ============ 变量 ============
WaveformType currentWaveType = WAVE_SQUARE;
unsigned long lastSampleTime = 0;
const unsigned long sampleIntervalUs = 1000000 / SAMPLE_RATE;  // 采样间隔(微秒)
bool lastButtonState = LOW; // 用于消抖的上次按钮状态
bool buttonState = LOW; // 当前按钮状态
long lastDebounceTime = 0; // 上次状态变化时间
const long debounceDelay = 50; // 消抖时间 (毫秒)

// ============ 函数声明 ============
void setupPins();
void updateAmplitude();
void printWaveformInfo();

// ============ 初始化 ============
void setup() {
  // 初始化串口
  Serial.begin(9600);
  Serial.println(F("=== Arduino wave signal generator ==="));
  
  // 初始化引脚
  setupPins();
  
  // 初始化波形生成器
  waveformGen.begin();
  
  // 初始化DAC
  dacOutput.begin();
  
  // 初始设置
  currentWaveType = WAVE_SQUARE;
  
  Serial.println(F("Initialization complete"));
  printWaveformInfo();
}

// ============ 主循环 ============
void loop() {
  unsigned long currentTime = micros();
  
  // 定时输出采样点
  if (currentTime - lastSampleTime >= sampleIntervalUs) {
    lastSampleTime = currentTime;
    
    // 1. 读取波形选择 (降低读取频率，每100ms读一次)
    static unsigned long lastSelectRead = 0;
    if (currentTime - lastSelectRead > 100000) {  // 100ms
      lastSelectRead = currentTime;
      
      int reading = digitalRead(WAVE_SELECT_PIN);
      if (reading != lastButtonState) { // Detect state change
        lastDebounceTime = currentTime;
      }
      if ((currentTime - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;
          if (buttonState == HIGH) {
            currentWaveType = waveformGen.updateWaveform(currentWaveType);
            printWaveformInfo();
          }
        }
        
      }
      lastButtonState = reading;
    }
    
    // 2. 更新幅度 (每50ms更新一次)
    static unsigned long lastAmpUpdate = 0;
    if (currentTime - lastAmpUpdate > 50000) {  // 50ms
      lastAmpUpdate = currentTime;
      updateAmplitude();
    }
    
    // 3. 生成下一个采样点并输出
    uint8_t sample = waveformGen.getNextSample(currentWaveType);
    dacOutput.write(sample);
    
    //Serial.print(F("Output sample: "));
    //Serial.println(sample);
  }
}

// ============ 辅助函数 ============

/**
 * 初始化所有引脚
 */
void setupPins() {
  // DAC数据引脚已在DACOutput中初始化
  // 设置波形选择引脚为输入
  pinMode(WAVE_SELECT_PIN, INPUT_PULLUP);
  pinMode(AMP_POT_PIN, INPUT);
}

/**
 * 从电位器读取并更新幅度
 * 幅度范围: 0 到 255 (8位DAC)
 */
void updateAmplitude() {
  int potValue = analogRead(AMP_POT_PIN);
  
  // 将0-1023映射到0-255
  float amplitude = 0.8* float(potValue)/511.0 +0.2; // 0.2-1.0之间线性变化
  
  waveformGen.setAmplitude(amplitude);
}

/**
 * 打印当前波形信息
 */
void printWaveformInfo() {
  // Serial.print(F("current wavetype: "));
  
  // switch (currentWaveType) {
  //   case WAVE_SQUARE:
  //     Serial.print(F("square"));
  //     break;
  //   case WAVE_TRIANGLE:
  //     Serial.print(F("triangle"));
  //     break;
  //   case WAVE_SINE:
  //     Serial.print(F("sine"));
  //     break;
  //   default:
  //     Serial.print(F("unknown"));
  // }
  
  // Serial.print(F(", frequency: "));
  // Serial.print(waveformGen.getCurrentFrequency());
  // Serial.println(F(" Hz"));

  // Serial.print(F("amplitude: "));
  // waveformGen.getCurrentAmplitude();
  waveformGen.getCurrentFrequency();
  Serial.println(F("\n========================="));
}