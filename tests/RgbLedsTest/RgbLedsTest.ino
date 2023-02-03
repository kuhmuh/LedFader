#line 2 "RgbLedsTest.ino"

#include <AUnit.h>
#include "RgbLeds.h"

using namespace aunit;

int analogWriteHit = 0;
int pinModeHit = 0;

void testAnalogWriteFunction(uint8_t pin, int value) {
  analogWriteHit++;
}

void testPinModeFunction(uint8_t pin, uint8_t mode) {
  pinModeHit++;
}

test(constructor) {
  RgbLeds leds = RgbLeds(1, 2, 3);

  assertEqual(leds.toString(), "{pins=[1, 2, 3], levels={red=0, green=0, blue=0}}");
}

test(init) {
  pinModeHit = 0;
  RgbLeds leds = RgbLeds(1, 2, 3);

  leds.init(testPinModeFunction, testAnalogWriteFunction);

  assertEqual(3, pinModeHit);
}

test(updateView) {
  analogWriteHit = 0;
  RgbLeds leds = RgbLeds(1, 2, 3);

  leds.init(testPinModeFunction, testAnalogWriteFunction);
  leds.updateView();

  assertEqual(3, analogWrite);
}

test(setLevels, set_correct_value) {
  RgbLeds leds = RgbLeds(1, 2, 3);

  leds.setLevels(0xff,0xfe,0xfd);

  assertEqual(0xff, leds.getRedLevel());
  assertEqual(0xfe, leds.getGreenLevel());
  assertEqual(0xfd, leds.getBlueLevel());
}

test(setLevels, cuts_values) {
  RgbLeds leds = RgbLeds(1, 2, 3);

  leds.setLevels(256,-1,255);

  assertEqual(255, leds.getRedLevel());
  assertEqual(0, leds.getGreenLevel());
  assertEqual(255, leds.getBlueLevel());
}

// ------------------------------------------------------
// The main body.
// ------------------------------------------------------

void setup() {
}

void loop() {
  TestRunner::run();
}