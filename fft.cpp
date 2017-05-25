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

#ifdef FFT_TEST
void test_fft() {
  Serial.println("test_fft(): Starting FFT Tests");
  Serial.println("test_fft(): Finished FFT Tests");
}
#endif
