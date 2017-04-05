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

// type to contain fixed point numbers
typedef int32_t fixed_t;

// scale to multiply a float with in order to get a fixed point number
// enob is 15 for each number with an entropy of ~11,1
// can hold 4 decimal digits exactly and 5 with numerical errors
const int32_t fixed_point_scale = 0x00001000;

// converts a float between -1 and 1 to a fixed point number
inline fixed_t fixed_from_float(float);

// converts a fixed point number to a float between -1 and 1
inline float fixed_to_float(fixed_t);

// returns the sum of the two arguments
inline fixed_t fixed_add(fixed_t, fixed_t);

// subtracts the second argument from the first
inline fixed_t fixed_sub(fixed_t, fixed_t);

// multiplies the two arguments
inline fixed_t fixed_mul(fixed_t, fixed_t);

// divides the first argument by the second
inline fixed_t fixed_div(fixed_t, fixed_t);

#endif
