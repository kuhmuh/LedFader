#line 2 "RgbLedsTest.ino"

#include <AUnit.h>
#include "RgbLeds.h"

using namespace aunit;

int analogWriteHit = 0;
int pinModeHit = 0;

void testAnalogWriteFunction(uint8_t, int) {
  analogWriteHit++;
}

void testPinModeFunction(uint8_t, uint8_t) {
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

  leds.setLevels(0xff, 0xfe, 0xfd);

  assertEqual(0xff, leds.getRedLevel());
  assertEqual(0xfe, leds.getGreenLevel());
  assertEqual(0xfd, leds.getBlueLevel());
}

test(setLevels, cuts_values) {
  RgbLeds leds = RgbLeds(1, 2, 3);

  leds.setLevels(256, -1, 255);

  assertEqual(255, leds.getRedLevel());
  assertEqual(0, leds.getGreenLevel());
  assertEqual(255, leds.getBlueLevel());
}

test(handleFade, does_nothing_if_outside_interval) {
  RgbLeds leds = RgbLeds(1, 2, 3);
  leds.setFadeInterval(10);
  leds.handleFade(9);

  assertEqual(0, leds.getRedLevel());
}

test(handleFade, adds_offset_if_after_interval) {
  RgbLeds leds = RgbLeds(1, 2, 3);
  leds.setFadeInterval(10);
  leds.startFading();
  leds.handleFade(11);

  assertEqual(5, leds.getRedLevel());
}

test(handleFade, does_nothing_if_after_interval_but_not_fading) {
  RgbLeds leds = RgbLeds(1, 2, 3);
  leds.setFadeInterval(10);
  leds.stopFading();
  leds.handleFade(11);

  assertEqual(0, leds.getRedLevel());
}

test(handleFade, decrease_on_max) {
  RgbLeds leds = RgbLeds(1, 2, 3);
  leds.setFadeInterval(10);
  leds.setLevels(250, 250, 250);
  leds.startFading();

  leds.handleFade(11);
  
  assertEqual(255, leds.getRedLevel());
  
  leds.handleFade(21);
  
  assertEqual(250, leds.getRedLevel());

  leds.handleFade(31);
  assertEqual(245, leds.getRedLevel());
}

test(handleFade, increase_on_min) {
  RgbLeds leds = RgbLeds(1, 2, 3);
  leds.setFadeInterval(10);
  leds.setLevels(255, 255, 255);
  leds.startFading();

  for (int i = 0; i < 50; i++) {
    leds.handleFade(i*10+1);
    assertEqual(255-i*5, leds.getRedLevel());
  }
  assertEqual(10, leds.getRedLevel());
  leds.handleFade(50*10+1);
  assertEqual(5, leds.getRedLevel());
  leds.handleFade(51*10+1);
  assertEqual(0, leds.getRedLevel());
  leds.handleFade(52*10+1);
  assertEqual(5, leds.getRedLevel());
}

void setup() {
}

void loop() {
  TestRunner::run();
}