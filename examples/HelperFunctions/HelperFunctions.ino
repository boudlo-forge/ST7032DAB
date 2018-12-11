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

double percentage = 0; // display percentage for the battery, signal and volume
double percentageIncrement = 0.05; // adjusts the speed of the strobing

unsigned long previousClockMillis = 0;
const long clockInterval = 1000;

void setup() {
  // Set the LCD Contrast
  lcd.init(33);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  // Show the clock digits and colon (Display 00:00)
  lcd.initClockDigits();
}

void loop() {
  // Make the signal meter go up and down
  lcd.setVolumePercent(percentage);

  // Make the signal meter go up and down
  lcd.setSignalPercent(percentage);

  // Do the same for the battery
  lcd.setBatteryPercent(percentage);

  // Incrementally increase and then decrease the output for the above
  // se go to 150 and -50 to give a pause at each end of the strobe
  if(percentage > 150 || percentage < -50) { percentageIncrement = 0-percentageIncrement; }
  percentage += percentageIncrement;

  // Leave this in the main loop and it will flash the time colon every second.
  lcd.flashTimeColon();

  // Leave this in the main loop and it will flash the alarm every second.
  lcd.flashAlarmIcon();

  // Update the clock(s) every second using delay-less timing
  unsigned long currentClockMillis = millis();
  unsigned long currentLCDMillis = millis();

  // is it time to update the time displays?
  if (currentClockMillis - previousClockMillis >= clockInterval) {
    previousClockMillis = currentClockMillis;
    int his[3]; // Array for Hours, Minutes, Seconds (H,i,s)
    lcd.millisToTime(currentClockMillis, his); // Convert the milliseconds to H,i,s
    lcd.writeTime(his[0], his[1]); // Write the Minutes and Seconds, ignore the Seconds

    // set the cursor to column 0, line 1
    // print the number of seconds since reset on the second line:
    char* timeStr = lcd.millisToString(currentLCDMillis);
    lcd.setCursor(0, 1);
    lcd.print(timeStr);
  }
}
