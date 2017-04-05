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
#include "fixedpoint.h"

// array of pointers to the filters
goertzel_t *filters[NUMBER_OF_BINS];

// filters are done
bool filters_done = false;

void alloc_filters() {
  // allocate memory for filters
  for (int i = 0; i < NUMBER_OF_BINS; i++) {
    filters[i] = (goertzel_t *) malloc(sizeof(goertzel_t));
  }
}

void init_filters() {
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
    uint16_t number_of_samples = 50;

    // call constructor
    goertzel_init(filters[NUMBER_OF_BINS - i - 1], normalized_bin_frequency,
                  number_of_samples);
  }
}

void setup() {
  // Serial connection for debugging output
  Serial.begin(115200);

  // tests
  #ifdef FIXED_TEST
  test_fixed_point();
  while(true) {}
  #endif

  // init goertzel filters
  alloc_filters();
  init_filters();

  // init timer for data acquisition
  init_timer();
}

void loop() {
  // do something, when filters are done
  if (filters_done) {
    // output spectrum
    for (int i = 0; i < NUMBER_OF_BINS; i++) {
      Serial.println(goertzel_get_magnitude(filters[i]));
    }

    // request new samples
    init_filters();
    filters_done = false;
    enable_timer();
  }
}

inline float acquire_sample() {
  return analogRead(A0);
}

inline void feed_filters(float sample) {
  // iterate over filters
  for (int i = 0; i < NUMBER_OF_BINS; i++) {
    goertzel_feed_sample(filters[i], sample);
  }
}

inline bool check_filters() {
  // iterate over filters, if one is not done, more samples are needed
  bool filters_done = true;
  for (int i = 0; i < NUMBER_OF_BINS; i++) {
    if (filters[i]->processed_samples < filters[i]->max_samples)
      filters_done = false;
  }

  return filters_done;
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

  // acquire sample data from adc
  float sample = acquire_sample();

  // process sample
  feed_filters(sample);

  // check if filters are done
  filters_done = check_filters();
  if (filters_done) {
    disable_timer();
  }

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

