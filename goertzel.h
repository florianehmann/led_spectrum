/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#ifndef _GOERTZEL__H_
#define _GOERTZEL__H_

#include "Arduino.h"
#include "fixedpoint.h"

typedef struct {
  // constants
  fixed_t cosomega;
  fixed_t sinomega;
  fixed_t two_cosomega;
  // intermediate sequence
  fixed_t s_n;
  fixed_t s_n_minus_1;
  fixed_t s_n_minus_2;
  // status variables
  uint16_t processed_samples;
  uint16_t max_samples;
} goertzel_t;

// init goertzel filter with normalized frequency
// i.e. phase increment per sample
// and number of samples to process
void goertzel_init(goertzel_t *filter, float omega, uint16_t max_samples);

// feed a sample to the filer
void goertzel_feed_sample(goertzel_t *filter, fixed_t sample);

// return magnitude
float goertzel_get_magnitude(goertzel_t *filter);

#endif
