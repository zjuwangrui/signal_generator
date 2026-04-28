#include <Arduino.h>

#define WAVE_SELECT_PIN 2

enum WaveformType {
  WAVE_SQUARE = 0,
  WAVE_TRIANGLE = 1,
  WAVE_SINE = 2,
  WAVE_TYPE_COUNT = 3
};

int currentWaveType = WAVE_SQUARE;
bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

int updateWaveform(int currentWaveform) {
  switch (currentWaveform) {
    case WAVE_SQUARE:
      return WAVE_TRIANGLE;
    case WAVE_TRIANGLE:
      return WAVE_SINE;
    case WAVE_SINE:
    default:
      return WAVE_SQUARE;
  }
}

const char *waveName(int waveType) {
  switch (waveType) {
    case WAVE_SQUARE:
      return "WAVE_SQUARE";
    case WAVE_TRIANGLE:
      return "WAVE_TRIANGLE";
    case WAVE_SINE:
      return "WAVE_SINE";
    default:
      return "UNKNOWN";
  }
}

void printWaveformInfo() {
  Serial.print("waveformType = ");
  Serial.print(currentWaveType);
  Serial.print(" -> ");
  Serial.println(waveName(currentWaveType));
}

void setup() {
  Serial.begin(115200);
  pinMode(WAVE_SELECT_PIN, INPUT_PULLUP);

  lastButtonState = digitalRead(WAVE_SELECT_PIN);
  buttonState = lastButtonState;

  Serial.println("waveformType single-file test");
  Serial.println("press/release button on D2 to cycle waveformType");
  printWaveformInfo();
}

void loop() {
  unsigned long currentTime = micros();

  static unsigned long lastSelectRead = 0;
  if (currentTime - lastSelectRead > 100000) {
    lastSelectRead = currentTime;

    int reading = digitalRead(WAVE_SELECT_PIN);
    if (reading != lastButtonState) {
      lastDebounceTime = currentTime;
    }

    if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == HIGH) {
          currentWaveType = updateWaveform(currentWaveType);
          printWaveformInfo();
        }
      }
    }

    lastButtonState = reading;
  }
}
