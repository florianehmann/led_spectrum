/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "goertzel.h"

void goertzel_init(goertzel_t *filter, float omega, uint16_t max_samples) {
  // init constants
  filter->cosomega = cos(omega);
  filter->sinomega = sin(omega);
  filter->two_cosomega = 2 * filter->cosomega;

  // init intermediate sequence
  filter->s_n = 0;
  filter->s_n_minus_1 = 0;
  filter->s_n_minus_2 = 0;

  // init status variables
  filter->processed_samples = 0;
  filter->max_samples = max_samples;
}

void goertzel_feed_sample(goertzel_t *filter, float sample) {
  // check if filter capacity is reached
  if (filter->processed_samples >= filter->max_samples) {
    return;
  }
  // compute intermediate sequence
  filter->s_n = sample + filter->two_cosomega * filter->s_n_minus_1 - filter->s_n_minus_2;

  // shit intermediate sequence
  filter->s_n_minus_2 = filter->s_n_minus_1;
  filter->s_n_minus_1 = filter->s_n;

  // update status variable
  filter->processed_samples++;
}

float goertzel_get_magnitude(goertzel_t *filter) {
  // compute dft component
  float real = filter->s_n_minus_1 - filter->cosomega * filter->s_n_minus_2;
  float imag = -1.0 * filter->sinomega * filter->s_n_minus_2;

  return sqrt(sq(real) + sq(imag));
}

