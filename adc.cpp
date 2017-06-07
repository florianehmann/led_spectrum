/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

#include "adc.h"
#include "led_spectrum.h"
#include "SPI.h"

void init_adc() {
  // init spi bus
  SPI.begin();
  SPI.beginTransaction(SPISettings(2800000, MSBFIRST, SPI_MODE0));

  // set the pin mode of the adc cs line to output
  pinMode(ADC_CS, OUTPUT);
  digitalWrite(ADC_CS, HIGH);

  // take first sample to fully power adc
  acquire_sample();
}

uint16_t acquire_sample() {
  // read from the bus
  digitalWrite(ADC_CS, LOW);
  uint16_t data = SPI.transfer16(0xFF00);

  // clean received data
  data = 0x03FF & (data >> 3);

  return data;
}

#ifdef ADC_TEST

void test_adc() {
  Serial.println("test_adc(): Testing ADC...");

  while (true) {
    Serial.println(acquire_sample(), DEC);
  }
}

#endif
