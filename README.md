LED Spectrum
============

Arduino Sketch for LED Matrix based Audio Visualizer.

Hardware
--------

Currently the code is designed to run on an ATmega328P MCU. Some of the hardware-specific features, like timers are used.

Computation
-----------

There was an attempt to compute the spectrum by using multiple Goertzel filters to process the data on the fly, but even with fixed point arithmetic the ATmega328P is not fast enough to process the data at the intended rate.

The new approach is to simply store samples and process them with a fast fourier transform algorithm, which also reaches a limit of the ATmega328 in terms of memory, but yields better results.

Disclaimer
---------

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) 2017 Florian Ehmann
