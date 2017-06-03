/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "fft.h"

// finds highest non-zero bit
int log2radix2(int n) {
  // sanity check n
  if (n <= 0) {
    return -1;
  }

  // currently examined bit
  int active_bit = sizeof(int) * 8 - 1;

  // go through the bits of n
  while (active_bit >= 0) {

    // check if bit is set
    if ((n >> active_bit & 1) > 0) {
      return active_bit;
    }

    active_bit--;
  }

  // should not happen
  return -2;
}

// reverses a number n with at most highest_bit+1 bits
int reverse(int n, int highest_bit) {
  // iterate over bits and store result
  int reversed = 0;
  int active_bit = highest_bit;
  while (active_bit >= 0) {
    // new position of active bit
    int new_pos = highest_bit - active_bit;

    // set bit in result
    reversed |= ((n >> active_bit) & 1) << (new_pos - 1);

    active_bit--;
  }

  return reversed;
}

// (bit) reverses an array
void reverse_array(fixed_t *x, int len, int highest_bit) {
  // reorder
  for (int i = 0; i < len; i++) {
    int j = reverse(i, highest_bit);
    if (j > i) {
      fixed_t temp = x[i];
      x[i] = x[j];
      x[j] = temp;
    }
  }
}

void fft(fixed_t *x_real, fixed_t *x_imag, int len) {
  // reverse input array
  reverse_array(x_real, len, log2radix2(len));

  for (int s = 1; s < log2radix2(len) + 1; s++) {
    int m = 1 << s;
    double phase = -2 * PI / m;
    fixed_t omega_m_real = fixed_from_float(cos(phase));
    fixed_t omega_m_imag = fixed_from_float(sin(phase));
    for (int k = 0; k < len; k += m) {
      fixed_t omega_real = fixed_point_scale;
      fixed_t omega_imag = 0;
      for (int j = 0; j < m / 2; j++) {
        fixed_t t_real = fixed_mul(omega_real, x_real[k + j + m / 2])
            - fixed_mul(omega_imag, x_imag[k + j + m / 2]);
        fixed_t t_imag = fixed_mul(omega_imag, x_real[k + j + m / 2])
            + fixed_mul(omega_real, x_imag[k + j + m / 2]);

        fixed_t u_real = x_real[k + j];
        fixed_t u_imag = x_imag[k + j];

        x_real[k + j] = u_real + t_real;
        x_imag[k + j] = u_imag + t_imag;

        x_real[k + j + m / 2] = u_real - t_real;
        x_imag[k + j + m / 2] = u_imag - t_imag;

        fixed_t omega_alt_real = omega_real;
        fixed_t omega_alt_imag = omega_imag;
        omega_real = fixed_mul(omega_alt_real, omega_m_real)
            - fixed_mul(omega_alt_imag, omega_m_imag);
        omega_imag = fixed_mul(omega_alt_real, omega_m_imag)
            + fixed_mul(omega_alt_imag, omega_m_real);
      }
    }
  }
}

float abscoeff(fixed_t re, fixed_t im) {
  float ref = fixed_to_float(re);
  float imf = fixed_to_float(im);
  return sqrt(pow(ref, 2) + pow(imf, 2));
}

#ifdef FFT_TEST

fixed_t sine(float t, float frequency) {
  float value = sin(TWO_PI * frequency * t) + 0.5 * sin(TWO_PI * frequency/2 * t);
  return fixed_from_float(value);
}

void test_fft() {
  Serial.println("test_fft(): Starting FFT Tests");

  // test with simple sines

  // constants and fields
  const int number_of_samples = 256;
  fixed_t *data_re = (fixed_t*) calloc(number_of_samples, sizeof(fixed_t));
  fixed_t *data_im = (fixed_t*) calloc(number_of_samples, sizeof(fixed_t));
  float sig_freq = 10;
  float max_freq = 20;
  float sample_freq = 2 * max_freq;
  float sample_spacing = 1 / sample_freq;
  float t = 0.0;

  // generate test data
  for (int i = 0; i < number_of_samples; i++) {
    data_re[i] = sine(t, sig_freq);
    t += sample_spacing;
  }

  // perform transformation
  fft(data_re, data_im, number_of_samples);

  // output results
  for (int i = 0; i < number_of_samples / 2; i++) {
    Serial.println(abscoeff(data_re[i], data_im[i]) / number_of_samples * 2);
  }

  Serial.println("test_fft(): Finished FFT Tests (should be verified with Serial Plotter)");
}

#endif
