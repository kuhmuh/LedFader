#ifndef RgbLeds_h
#define RgbLeds_h

#include "Arduino.h"

typedef std::function<void(uint8_t, int)> AnalogWriteFunction;
typedef std::function<void(uint8_t, uint8_t)> PinModeFunction;

class RgbLeds {
public:
  const uint redPin;
  const uint greenPin;
  const uint bluePin;

  RgbLeds(uint _redPin, uint _greenPin, uint _bluePin);

  void init(PinModeFunction pinMode, AnalogWriteFunction analogWrite);
  String toString();
  void updateView();
  void setLevels(int r, int g, int b);
  int getRedLevel();
  int getGreenLevel();
  int getBlueLevel();

private:
  AnalogWriteFunction analogWrite;
  int redLevel = 0;
  int greenLevel = 0;
  int blueLevel = 0;
};
#endif