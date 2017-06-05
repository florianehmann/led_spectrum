/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#ifndef _MATRIX__H_
#define _MATRIX__H_

#include "Arduino.h"

// Library for LED Matrix Control
// https://github.com/wayoda/LedControl/releases

// switch for fft tests
#define MATRIX_TEST
#include "LedControl.h"

// pins for the matrix
#define DATA_IN 4
#define CLK     5
#define CS      6

extern LedControl matrix;

void init_matrix();

#ifdef MATRIX_TEST

void test_matrix();

#endif

#endif
