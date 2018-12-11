/*
  ST7032 DAB Radio LCD Module
  By Jim Rouse, Boudlo Forge Ltd
  jim@boudlo.gg
  License: MIT

  Built on
  ST7032 I2C LCD Library
  by tomozh@gmail.com

  Which was built on
  Arduino LiquidCrystal Library
*/

#include "ST7032DAB.h"

// Change this to whichever io pin you are using to reset the board, default is 5
// #define RESET_MCU_PIN     5

ST7032DAB lcd;

void setup() {
  // Initialize the LCD
  lcd.init(33); // Contrast 33 (good for 3.3v)
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // Main Loop
}
