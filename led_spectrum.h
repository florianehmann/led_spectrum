/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#ifndef _LED_SPECTRUM__H_
#define _LED_SPECTRUM__H_

#include "Arduino.h"

// desired maximum frequency in the spectrum in Hz
// the current prescaling of the counter does only allow
// values between 20 kHz and 1954 Hz
const unsigned int MAX_FREQ = 5000;

// number of frequency bins of the output, e.g. 8 for an 8x8 matrix
const uint8_t NUMBER_OF_BINS = 8;

// time in ms between two matrix updates
const unsigned long MATRIX_PERIOD = 100;

// sample frequency in Hz
const unsigned int SAMPLE_FREQ = 2 * MAX_FREQ;

// distance of two samples in the time domain in us
const unsigned int SAMPLE_SPACING = (unsigned int) (1000000l / SAMPLE_FREQ);

// acquire a sample from the adc
// do this inline, to save time on stack operations
inline void acquire_sample();

// perform steps necessary to update the matrix
void update_matrix();

// initializes the timer for the data acquisition
void init_timer();

#endif
