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
#include "goertzel.h"

goertzel_t *filters[NUMBER_OF_BINS];

void init_filters() {
  // allocate memory for filters
  for (int i = 0; i < NUMBER_OF_BINS; i++) {
    filters[i] = (goertzel_t *) malloc(sizeof(goertzel_t));
  }

  // calculate the factor q between two frequency bins in the relation
  // f_n = f_(n-1) * q, when n is the number of the bin
  float q = pow(((float) MAX_FREQ) / MIN_FREQ, 1.0 / (NUMBER_OF_BINS - 1));

  // initialize filter constants and sequences
  // start with highest frequency, to avoid exceeding the maximum
  // frequency, due to numerical errors
  for (int i = 0; i < NUMBER_OF_BINS; i++) {
    // calculate the bin frequency
    float factor = pow(q, i);
    float bin_frequency = ((float) MAX_FREQ) / factor;
    float normalized_bin_frequency = TWO_PI * bin_frequency / SAMPLE_FREQ;

    // TODO calculate the number of samples to match the constant Q
    uint16_t number_of_samples = 100;

    // call constructor
    goertzel_init(filters[NUMBER_OF_BINS - i], normalized_bin_frequency,
                  number_of_samples);
  }
}

void setup() {
  // Serial connection for debugging output
  Serial.begin(115200);

  // init timer for data acquisition
  init_timer();

  // init goertzel filters
  init_filters();
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

inline void acquire_sample() {
  float sample = analogRead(A0);
}

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

