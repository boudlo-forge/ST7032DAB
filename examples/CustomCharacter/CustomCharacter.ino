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

#include <ST7032DAB.h>

// initialize the library
ST7032DAB lcd;

// make some custom characters:
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

void setup() {
  // set up the lcd with 33 contrast
  lcd.init(33);

  // create a new character
  lcd.createChar(0, heart);
  // create a new character
  lcd.createChar(1, smiley);
  // create a new character
  lcd.createChar(2, frownie);
  // create a new character
  lcd.createChar(3, armsDown);
  // create a new character
  lcd.createChar(4, armsUp);

  lcd.setCursor(1, 0);
  // Print a message to the lcd.
  lcd.print("I ");
  lcd.write(0);
  lcd.print(" Arduino! ");
  lcd.write(1);

  lcd.setCursor(5, 1);
  // Print the other icons we made
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);

}

void loop() {
    // Main Loop
}
