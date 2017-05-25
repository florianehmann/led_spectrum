/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#ifndef _FIXEDPOINT__H_
#define _FIXEDPOINT__H_

#include "Arduino.h"

// switch for fixed point tests
//#define FIXED_TEST

// type to contain fixed point numbers
typedef int16_t fixed_t;

// scale to multiply a float with in order to get a fixed point number
const int16_t fixed_point_scale = (1L << 8);

// two times the capacity of the fixed_t for multiplication
typedef int32_t double_fixed_t;

// converts a float between -2 and 2 to a fixed point number
inline fixed_t fixed_from_float(float);

// converts a fixed point number to a float between -2 and 2
inline float fixed_to_float(fixed_t);

// returns the sum of the two arguments
inline fixed_t fixed_add(fixed_t, fixed_t);

// subtracts the second argument from the first
inline fixed_t fixed_sub(fixed_t, fixed_t);

// multiplies the two arguments
inline fixed_t fixed_mul(fixed_t, fixed_t);

// divides the first argument by the second
inline fixed_t fixed_div(fixed_t, fixed_t);

inline fixed_t fixed_from_float(float f) {
  return (fixed_t) (f * fixed_point_scale);
}

inline float fixed_to_float(fixed_t f) {
  float fl = f;
  return fl / fixed_point_scale;
}

inline fixed_t fixed_add(fixed_t a, fixed_t b) {
  return a + b;
}

inline fixed_t fixed_sub(fixed_t a, fixed_t b) {
  return a - b;
}

inline fixed_t fixed_mul(fixed_t a, fixed_t b) {
  double_fixed_t result = a;
  result = (result * b) / fixed_point_scale;
  return (fixed_t) result;
}

inline fixed_t fixed_div(fixed_t a, fixed_t b) {
  double_fixed_t result = a;
  result = (result * fixed_point_scale) / b;
  return (fixed_t) result;
}

// test functions
#ifdef FIXED_TEST
void test_fixed_point();
#endif

#endif
