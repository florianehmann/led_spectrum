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
#include "LedControl.h"

// switch for fft tests
//#define MATRIX_TEST

extern LedControl matrix;

// initializes the matrix controller
void init_matrix();

// updates the matrix according to the bar data array
void update_bars();

#ifdef MATRIX_TEST

void test_matrix();

void simple_matrix_test();

void test_matrix_bars();

#endif

#endif
