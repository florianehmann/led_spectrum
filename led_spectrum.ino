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
#include "adc.h"

// sampled audio data
volatile uint16_t *audio_data;

// number of samples in audio_data array
volatile uint16_t acquired_samples;

// spectrum arrays
fixed_t *audio_re;
fixed_t *audio_im;

// flag for complete capture
volatile bool capture_complete;

// bar data array for the led matrix
uint8_t bar_data[NUMBER_OF_BINS];

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

  // init array for sampled audio data
  audio_data = new uint16_t[NUMBER_OF_SAMPLES];
  acquired_samples = 0;
  capture_complete = false;

  // init timer for data acquisition
  Serial.println("setup(): Initializing Timer");
  init_timer();

  // init adc
  Serial.println("setup(): Initializing ADC");
  init_adc();

  // init LED matrix
  Serial.println("setup(): Initializing LED Matrix");
  init_matrix();

  // matrix test after initialization
  #ifdef MATRIX_TEST
  test_matrix();
  #endif

  // adc test after initialization
  #ifdef ADC_TEST
  disable_timer();
  test_adc();
  #endif

  Serial.println("setup(): Setup complete");
}

void loop() {
  if (capture_complete) {
    // convert data to fixed float
    audio_re = new fixed_t[NUMBER_OF_SAMPLES];
    for (uint16_t i = 0; i < NUMBER_OF_SAMPLES; i++) {
      // convert to fixed point
      audio_re[i] = audio_data[i]; // no scaling because it's already over 1<<8
      //Serial.println(audio_data[i]);
    }

    // generate imaginary array
    delete audio_data; // free some heap
    audio_im = new fixed_t[NUMBER_OF_SAMPLES];

    // fourier transform
    fft(audio_re, audio_im, NUMBER_OF_SAMPLES);

    // output spectrum
    //for (uint16_t i = 0; i < NUMBER_OF_SAMPLES/2; i++) {
    //  Serial.println(abscoeff(audio_re[i], audio_im[i]));
    //}

    // compute bars and update matrix
    compute_bars();
    update_matrix();

    // get ready for next capture round
    delete audio_re;
    delete audio_im;
    audio_data = new uint16_t[NUMBER_OF_SAMPLES];
    //capture_complete = false;
    enable_timer();
  }
}

inline int simulate_sample() {
  static long time = 0;
  time += SAMPLE_SPACING;
  return sin(TWO_PI*500*time/1e6)*512;
}

void compute_bars() {
  // divide spectrum in NUMBER_OF_SAMPLES + 1 equal parts
  float part_width = NUMBER_OF_SAMPLES / 2.0 / (NUMBER_OF_BINS + 1);

  // compute part averages, omitting the part containing the dc component
  float sum[NUMBER_OF_BINS];
  for (uint16_t part = 0; part < NUMBER_OF_BINS; part++) {
    // compute start and end index of the part
    int start_index = (part + 1) * part_width;
    int end_index = (part + 2) * part_width - 1;

    // average the spectrum
    sum[part] = 0.0;
    for (int i = start_index; i < end_index; i++) {
      sum[part] += abscoeff(audio_re[i], audio_im[i]);
    }
  }

  // normalize bars
  float max = 0.0;
  for (uint16_t i = 0; i < NUMBER_OF_BINS; i++) {
    if (max < sum[i]) {
      max = sum[i];
    }
  }

  // filter max
  static float cap = 100.0;
  static int cap_depth = 10;
  cap = cap / cap_depth * (cap_depth -1) + max / cap_depth;
  for (uint16_t i = 0; i < NUMBER_OF_BINS; i++) {
    sum[i] /= cap;
    bar_data[i] = constrain(round(8 * sum[i]), 0, 8);
  }
}

void update_matrix() {
  // update bar data in the matrix
  update_bars();
}

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

  // clear timer
  //TCNT2 = 0;

  // acquire sample data from adc
  uint16_t sample = acquire_sample();

  // store sample
  if (acquired_samples < NUMBER_OF_SAMPLES) {
    audio_data[acquired_samples] = sample;
    acquired_samples++;
    Serial.println(sample);
  } else {
    disable_timer();
    acquired_samples = 0;
  }

  // restore status register
  SREG = old_SREG;
}

void disable_timer() {
  // disable output compare interrupt A
  TIMSK2 &= ~(1 << OCIE2A);
  capture_complete = true;
}

void enable_timer() {
  // enable output compare interrupt A
  TIMSK2 |= (1 << OCIE2A);
  capture_complete = false;
}

