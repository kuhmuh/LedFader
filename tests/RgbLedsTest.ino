#line 2 "RgbLedsTest.ino"

#include <AUnit.h>
#include "RgbLeds.h"

using namespace aunit;

test(constructor) {
  RgbLeds leds = RgbLeds(1, 2, 3);

  assertEqual(leds.toString(), "Rgb(red=0, green=0, blue=0)");
}

// ------------------------------------------------------
// The main body.
// ------------------------------------------------------

void setup() {
}

void loop() {
  TestRunner::run();
}