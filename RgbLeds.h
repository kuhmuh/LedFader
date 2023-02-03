#ifndef RgbLeds_h
#define RgbLeds_h

#include <Arduino.h>

typedef std::function<void(uint8_t, int)> AnalogWriteFunction;
typedef std::function<void(uint8_t, uint8_t)> PinModeFunction;

class RgbLeds {
public:
  const uint8_t redPin;
  const uint8_t greenPin;
  const uint8_t bluePin;

  RgbLeds(uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin);

  void init(PinModeFunction pinMode, AnalogWriteFunction analogWrite);
  String toString();
  void setLevels(int r, int g, int b);
  int getRedLevel();
  int getGreenLevel();
  int getBlueLevel();
  void startFading();
  void stopFading();
  void setFadeInterval(int newInterval);
  void handleFade(unsigned long currentMillis);
  void updateView();

private:
  AnalogWriteFunction analogWrite;
  int redLevel = 0;
  int greenLevel = 0;
  int blueLevel = 0;

  unsigned long previousMillis = 0;
  uint8_t interval = 50;
  int offset = 5;  // how many points to fade the LED by
  bool fading = false;

  int trim(int value);
};
#endif