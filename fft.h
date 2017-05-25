/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#ifndef _FFT__H_
#define _FFT__H_

#include "Arduino.h"
#include "fixedpoint.h"

// switch for fft tests
//#define FFT_TEST

// finds highest non-zero bit
int log2radix2(int n);

// reverses a number n with at most highest_bit+1 bits
int reverse(int n, int highest_bit);

// (bit) reverses an array
void reverse_array(fixed_t *x, int len, int highest_bit);

// inplace radix-2 cooley-tukey fft
void fft(fixed_t *x_real, fixed_t *x_imag, int len);

// gives the abs value of a coefficient
float abscoeff(fixed_t re, fixed_t im);

#ifdef FFT_TEST
extern void test_fft();
#endif

#endif
