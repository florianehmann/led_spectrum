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

// pins for the matrix
#define DATA_IN 4
#define CLK     5
#define CS      6

// pins for the adc
#define ADC_CS  7 // do not change, fixed in adc driver

// desired maximum frequency in the spectrum in Hz
// the current prescaling of the counter does only allow
// values between 20 kHz and 1954 Hz
const unsigned int MAX_FREQ = 10000;

// number of frequency bins of the output, e.g. 8 for an 8x8 matrix
const uint8_t NUMBER_OF_BINS = 8;

// number of the samples of the fft
const uint16_t NUMBER_OF_SAMPLES = 64;

// sample frequency in Hz
const unsigned int SAMPLE_FREQ = 2 * MAX_FREQ;

// distance of two samples in the time domain in us
const unsigned int SAMPLE_SPACING = (unsigned int) (1000000l / SAMPLE_FREQ);

// bar data for the led matrix
extern uint8_t bar_data[NUMBER_OF_BINS];

// acquire a sample from simulation
// do this inline, to save time on stack operations
inline int simulate_sample();

// compute bar data for the matrix
void compute_bars();

// perform steps necessary to update the matrix
void update_matrix();

// initializes the timer for the data acquisition
void init_timer();

// disable the timer/counter 2
void disable_timer();

// enable timer/counter 2
void enable_timer();

#endif
