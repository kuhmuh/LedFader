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

// ------------------------------------------------------
// The main body.
// ------------------------------------------------------

void setup() {
}

void loop() {
  TestRunner::run();
}