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

LedControl matrix = LedControl(DATA_IN, CLK, CS, 1);

void init_matrix() {
  // set brightness
  matrix.setIntensity(0, 1);

  // wake up MAX7219
  matrix.shutdown(0, false);
}

#ifdef MATRIX_TEST


void test_matrix() {
  Serial.println("test_matrix(): Testing LED Matrix...");
}

#endif
