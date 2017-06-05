/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "led_spectrum.h"
#include "fixedpoint.h"
#include "fft.h"
#include "matrix.h"

void setup() {
  // Serial connection for debugging output
  Serial.begin(115200);

  // tests
  #ifdef FIXED_TEST
  test_fixed_point();
  while(true) {}
  #endif

  #ifdef FFT_TEST
  test_fft();
  while(true) {}
  #endif

  // init timer for data acquisition
  Serial.println("setup(): Initializing timer");
  init_timer();

  // init LED matrix
  Serial.println("setup(): Initializing LED Matrix");
  init_matrix();

  // matrix test after initialization
  #ifdef MATRIX_TEST
  test_matrix();
  #endif
}

void loop() {
}

inline int acquire_sample() {
  static long time = 0;
  time += SAMPLE_SPACING;
  return sin(TWO_PI*500*time/1e6)*512;
}

void update_matrix() {}

void init_timer() {
  // Timer/Counter 2 in CTC mode
  TCCR2A |= (1 << WGM21);

  // prescaling to 1/32 of system clock
  TCCR2B &= ~(1 << CS22);
  TCCR2B |= (1 << CS21) | (1 << CS20);

  // enable output compare interrupt A
  TIMSK2 |= (1 << OCIE2A);

  // set TOP of counter according to sample rate
  OCR2A = (uint8_t) SAMPLE_SPACING / 32.0 * 16;
}

// timer interrupt service routine
ISR(TIMER2_COMPA_vect) {
  // save status register and disable interrupts globally
  uint8_t old_SREG = SREG;
  cli();

  // check if enough samples and disable timer is so

  // acquire sample data from adc
  acquire_sample();

  // store sample

  // restore status register
  SREG = old_SREG;
}

void disable_timer() {
  // disable output compare interrupt A
  TIMSK2 &= ~(1 << OCIE2A);
}

void enable_timer() {
  // enable output compare interrupt A
  TIMSK2 |= (1 << OCIE2A);
}

