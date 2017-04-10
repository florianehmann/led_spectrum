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
  filter->cosomega = fixed_from_float(cos(omega));
  filter->sinomega = fixed_from_float(sin(omega));
  filter->two_cosomega = 2 * filter->cosomega;

  // init intermediate sequence
  filter->s_n = fixed_from_float(0);
  filter->s_n_minus_1 = fixed_from_float(0);
  filter->s_n_minus_2 = fixed_from_float(0);

  // init status variables
  filter->processed_samples = 0;
  filter->max_samples = max_samples;
}

void goertzel_feed_sample(goertzel_t *filter, fixed_t sample) {
  // check if filter capacity is reached
  if (filter->processed_samples >= filter->max_samples) {
    return;
  }

  // compute intermediate sequence
  filter->s_n = fixed_sub(
                  fixed_add(sample,
                            fixed_mul(filter->two_cosomega, filter->s_n_minus_1)),
                  filter->s_n_minus_2);

  // shift intermediate sequence
  filter->s_n_minus_2 = filter->s_n_minus_1;
  filter->s_n_minus_1 = filter->s_n;

  // update status variable
  filter->processed_samples++;
}

float goertzel_get_magnitude(goertzel_t *filter) {
  // compute dft component
  float real = fixed_from_float(
                 fixed_sub(filter->s_n_minus_1,
                           fixed_mul(filter->cosomega, filter->s_n_minus_2)));
  float imag = fixed_from_float(
                 fixed_mul(
                   fixed_mul(fixed_from_float(-1.0), filter->sinomega),
                   filter->s_n_minus_2));

  return sqrt(sq(real) + sq(imag)) / pow(filter->max_samples, 2);
}

