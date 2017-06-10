/*
  The following code is part of the github repository
  https://github.com/florianehmann/led_spectrum

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
  PARTICULAR PURPOSE.

  Copyright (c) 2017 Florian Ehmann
*/

/*
  This Code is designed for the Microchip MCP3001 ADC
*/

#include "Arduino.h"

#ifndef _ADC__H_
#define _ADC__H_

// switch for adc tests
//#define ADC_TEST

// initialize adc
void init_adc();

// read an analog sample from the adc
uint16_t acquire_sample();

#ifdef ADC_TEST

void test_adc();

#endif

#endif
