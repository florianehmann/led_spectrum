/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "fixedpoint.h"

#ifdef FIXED_TEST

const float diff = 0.000001;

float random_float() {
  long scale = 500000L;
  long r = random(2 * scale + 1) - scale;
  return (float) r / scale;
}

void test_conversion() {
  for (int i = 0; i < 100; i++) {
    // generate random number
    float test_float = random_float();

    // convert random to fixed point
    fixed_t test_converted = fixed_from_float(test_float);

    // convert fixed point back to float
    float reconverted_float = fixed_to_float(test_converted);

    if (abs(test_float - reconverted_float > diff)) {
      Serial.print("test_conversion(): Failed at ");
      Serial.print(test_float, 4);
      Serial.print(" to ");
      Serial.print(reconverted_float, 4);
      Serial.print(" via ");
      Serial.print((long) test_converted, 4);
      Serial.print("\n");
    }
  }
}

void test_add() {
  for (int i = 0; i < 100; i++) {
    // generate random floats
    float a = random_float();
    float b = random_float();

    // fix point the random floats
    fixed_t a_fix = fixed_from_float(a);
    fixed_t b_fix = fixed_from_float(b);

    // perform computation
    float result = a + b;
    fixed_t result_fix = fixed_add(a_fix, b_fix);
    float result_converted = fixed_to_float(result_fix);

    if (abs(result - result_converted) > diff) {
      Serial.print("test_add(): Failed at ");
      Serial.print(a, 4);
      Serial.print(" + ");
      Serial.print(b, 4);
      Serial.print(" = ");
      Serial.print(result_converted, 4);
      Serial.print("\n");
    }
  }
}

void test_sub() {
  for (int i = 0; i < 100; i++) {
    // generate random floats
    float a = random_float();
    float b = random_float();

    // fix point the random floats
    fixed_t a_fix = fixed_from_float(a);
    fixed_t b_fix = fixed_from_float(b);

    // perform computation
    float result = a - b;
    fixed_t result_fix = fixed_sub(a_fix, b_fix);
    float result_converted = fixed_to_float(result_fix);

    if (abs(result - result_converted) > diff) {
      Serial.print("test_sub(): Failed at ");
      Serial.print(a, 4);
      Serial.print(" - ");
      Serial.print(b, 4);
      Serial.print(" = ");
      Serial.print(result_converted, 4);
      Serial.print("\n");
    }
  }
}

void test_mul() {
  for (int i = 0; i < 100; i++) {
    // generate random floats
    float a = random_float();
    float b = random_float();

    // fix point the random floats
    fixed_t a_fix = fixed_from_float(a);
    fixed_t b_fix = fixed_from_float(b);

    // perform computation
    float result = a * b;
    fixed_t result_fix = fixed_mul(a_fix, b_fix);
    float result_converted = fixed_to_float(result_fix);

    if (abs(result - result_converted) > diff) {
      Serial.print("test_mul(): Failed at ");
      Serial.print(a, 4);
      Serial.print(" * ");
      Serial.print(b, 4);
      Serial.print(" = ");
      Serial.print(result_converted, 4);
      Serial.print("\n");
    }
  }
}

void test_div() {
  for (int i = 0; i < 100; i++) {
    // generate random floats
    float a = random_float();
    float b = random_float();

    // fix point the random floats
    fixed_t a_fix = fixed_from_float(a);
    fixed_t b_fix = fixed_from_float(b);

    // perform computation
    float result = a / b;
    if (abs(result) > 1)
      break;
    fixed_t result_fix = fixed_div(a_fix, b_fix);
    float result_converted = fixed_to_float(result_fix);

    // check with higher tolerance, because of numerical errors of the denominator
    if (abs(result - result_converted) > diff * 10) {
      Serial.print("test_div(): Failed at ");
      Serial.print(a, 4);
      Serial.print(" / ");
      Serial.print(b, 4);
      Serial.print(" = ");
      Serial.print(result_converted, 4);
      Serial.print(" (should be ");
      Serial.print(result, 4);
      Serial.print(")");
      Serial.print("\n");
    }
  }
}

void test_fixed_point() {
  // pick up noise
  randomSeed(analogRead(A1));

  // start tests
  Serial.println("test_fixed_point(): Starting tests");
  test_conversion();
  test_add();
  test_sub();
  test_mul();
  test_div();
  Serial.println("test_fixed_point(): Finished tests");
}

#endif

