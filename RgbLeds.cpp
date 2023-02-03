#include "Arduino.h"
#include "RgbLeds.h"

RgbLeds::RgbLeds(uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin)
  : redPin(_redPin),
    greenPin(_greenPin),
    bluePin(_bluePin) {
}

void RgbLeds::init(PinModeFunction pinModeFunc, AnalogWriteFunction analogWrite) {
  pinModeFunc(redPin, OUTPUT);
  pinModeFunc(greenPin, OUTPUT);
  pinModeFunc(bluePin, OUTPUT);
  this->analogWrite = analogWrite;
}

String RgbLeds::toString() {
  return String("{pins=[") + redPin + ", " + greenPin + ", " + bluePin + "], levels={" + "red=" + redLevel + ", green=" + greenLevel + ", blue=" + redLevel + "}}";
}

void RgbLeds::updateView() {
  this->analogWrite(redPin, redLevel);
  this->analogWrite(greenPin, greenLevel);
  this->analogWrite(bluePin, blueLevel);
}

void RgbLeds::setLevels(int r, int g, int b) {
  redLevel = trim(r);
  greenLevel = trim(g);
  blueLevel = trim(b);
}

int RgbLeds::trim(int value) {
  if (value > 255) return 255;
  if (value < 0) return 0;
  return value;
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
