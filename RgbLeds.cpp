#include "Arduino.h"
#include "RgbLeds.h"

RgbLeds::RgbLeds(uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin)
  : redPin(_redPin),
    greenPin(_greenPin),
    bluePin(_bluePin) {
  this->analogWrite = analogWrite;
}

void RgbLeds::init(PinModeFunction pinModeFunc, AnalogWriteFunction analogWrite) {
  pinModeFunc(redPin, OUTPUT);
  pinModeFunc(greenPin, OUTPUT);
  pinModeFunc(bluePin, OUTPUT);
}

String RgbLeds::toString() {
  return String("Rgb(red=") + String(redLevel) + ", green=" + String(greenLevel) + ", blue=" + String(redLevel) + ")";
}

void RgbLeds::updateView() {
  this->analogWrite(redPin, redLevel);
  this->analogWrite(greenPin, greenLevel);
  this->analogWrite(bluePin, blueLevel);
}

void RgbLeds::setLevels(int r, int g, int b) {
  redLevel = r;
  greenLevel = g;
  blueLevel = b;
}

int RgbLeds::getRedLevel() {
  return redLevel;
}

int RgbLeds::getGreenLevel() {
  return greenLevel;
}

int RgbLeds::getBlueLevel() {
  return blueLevel;
}
