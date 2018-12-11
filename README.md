# ST7032DAB
Arduino Library and examples for CI064-4022-03 i2c LCD Modules designed for DAB Digital Radios. Helper functions are included for setting volume, battery and signal strength by percentage, as well bars. There are also helper functions for setting the 7-segment clock. Most helper functions have been written to minimise unnecessary i2c writes.

The CI064-4022-03 is an application specific 1602 LCD designed for DAB digital radios, as such it has icons for all sorts of useful functions. Notably a signal strength, battery indicator and separate 7-segment clock which can free up valuable character real estate for IoT projects. The i2c interface is provided by an ST7032, and this library is based on the good work of https://github.com/tomozh


3.3V    --+-- VDD
D5      ----- RES
SDA     --*-- SDA
SCL     --*-- SCL
GND     ----- GND

*... 4.7-10k pull-up resistor may be required.
