/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "fixedpoint.h"

inline fixed_t fixed_from_float(float f) {
  return (fixed_t) f * fixed_point_scale;
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
  return (a * b) / fixed_point_scale;
}

inline fixed_t fixed_div(fixed_t a, fixed_t b) {
  return (a * fixed_point_scale) / b;
}

