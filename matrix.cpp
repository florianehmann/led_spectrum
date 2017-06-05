/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "matrix.h"

// set number of controllers to 4, because test module has 4 matrices
LedControl matrix = LedControl(DATA_IN, CLK, CS, 4);

void init_matrix() {
  // set brightness
  matrix.setIntensity(0, 1);

  // wake up MAX7219
  matrix.shutdown(0, false);
}

#ifdef MATRIX_TEST

void test_matrix() {
  Serial.println("test_matrix(): Testing LED Matrix...");

  // coordinates of the led to show
  static int x, y = 0;

  // timing variable
  static unsigned long last_exec = millis();
  static unsigned long DELAY = 100;

  while (true) {
    // check if it's time to go to the next led
    unsigned int current_time = millis();
    if (current_time - last_exec > DELAY) {
      // set new execution time
      last_exec = current_time;

      // activate led
      matrix.clearDisplay(0);
      matrix.setLed(0, x, y, true);

      // increment coordinates
      x++;
      if (x > 7) {
        x = 0;
        y++;
      }
      if (y > 7) {
        y = 0;
      }
    }
  }
}

#endif
