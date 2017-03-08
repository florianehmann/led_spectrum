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

void setup() {
  // Serial connection for debugging output
  Serial.begin(115200);

  // init timer for data acquisition
  init_timer();
}

void loop() {
  // time of last matrix refresh
  static unsigned long last_matrix_update = millis();

  // current time
  unsigned long current_time = millis();

  // task timing
  if (current_time - last_matrix_update > MATRIX_PERIOD) {
    update_matrix();
    last_matrix_update = current_time;
  }
}

inline void acquire_sample() {}

void update_matrix() {}

void init_timer() {
  // Timer/Counter 2 in CTC mode
  TCCR2A |= (1 << WGM21);

  // prescaling to 1/32 of system clock
  TCCR2B |= (1 << CS21) | (1 << CS20);

  // enable output compare interrupt A
  TIMSK2 |= (1 << OCIE2A);

  // set TOP of counter according to sample rate
  OCR2A = (uint8_t) SAMPLE_SPACING / 8.0 / 16;
}

// timer interrupt service routine
ISR(TIMER2_COMPA_vect) {
  // save status register and disable interrupts globally
  uint8_t old_SREG = SREG;
  cli();

  // perform action
  acquire_sample();

  // restore status register
  SREG = old_SREG;
}

