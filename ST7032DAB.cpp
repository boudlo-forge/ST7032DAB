/*
  ST7032DAB.cpp - Arduino LiquidCrystal compatible library
  Original source is Arduino LiquidCrystal library
  Author:  tomozh@gmail.com
  License: MIT

  History:
    2014.10.13 コントラスト値のbit7がBONビットに影響する不具合を修正
    2014.08.23 コンストラクタでI2Cアドレスを設定可能にした
    2013.05.21 1st release

*/

#include "ST7032DAB.h"
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

// Array stores the persistent bit values for the 16 icon addresses.
int icon_addr_bits[16];

uint8_t currentVolume;

int volume_bars_1[3] = {
  ICON_BIT_VOLUME_1_BAR_1,
  ICON_BIT_VOLUME_1_BAR_2,
  ICON_BIT_VOLUME_1_BAR_3,
};

int volume_bars_2[3] = {
  ICON_BIT_VOLUME_2_BAR_4,
  ICON_BIT_VOLUME_2_BAR_5,
  ICON_BIT_VOLUME_2_BAR_6,
};

uint8_t currentBatteryStrength;

int battery_bars[4] = {
  ICON_BIT_BATTERY_1,
  ICON_BIT_BATTERY_2,
  ICON_BIT_BATTERY_3,
  ICON_BIT_BATTERY_4
};

uint8_t currentSignalStrength;

int strength_bars_2[5] = {
  ICON_BIT_SIGNAL_2_BAR_2,
  ICON_BIT_SIGNAL_2_BAR_3,
  ICON_BIT_SIGNAL_2_BAR_4,
  ICON_BIT_SIGNAL_2_BAR_5,
  ICON_BIT_SIGNAL_2_BAR_6
};

char currentDisplayedTime[4];
uint8_t timeColonState;
uint8_t alarmIconState;

uint8_t firstDigit;
uint8_t secondDigit;
uint8_t thirdDigit;
uint8_t fourthDigit;

// private methods

void ST7032DAB::setDisplayControl(uint8_t setBit) {
  _displaycontrol |= setBit;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void ST7032DAB::resetDisplayControl(uint8_t resetBit) {
  _displaycontrol &= ~resetBit;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void ST7032DAB::setEntryMode(uint8_t setBit) {
  _displaymode |= setBit;
  command(LCD_ENTRYMODESET | _displaymode);
}

void ST7032DAB::resetEntryMode(uint8_t resetBit) {
  _displaymode &= ~resetBit;
  command(LCD_ENTRYMODESET | _displaymode);
}

void ST7032DAB::normalFunctionSet() {
  command(LCD_FUNCTIONSET | _displayfunction);
}

void ST7032DAB::extendFunctionSet() {
  command(LCD_FUNCTIONSET | _displayfunction | LCD_EX_INSTRUCTION);
}

// public methods

ST7032DAB::ST7032DAB(int i2c_addr)
  : _displaycontrol(0x00)
  , _displaymode(0x00)
  , _i2c_addr((uint8_t)i2c_addr) {}

void ST7032DAB::init(uint8_t contrast) {
  begin(16,2);
  setContrast(contrast);
}

void ST7032DAB::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {

  _displayfunction  = LCD_8BITMODE | LCD_1LINE | LCD_5X8DOTS;

  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5X10DOTS;
  }

  #ifdef RESET_MCU_PIN
    pinMode(RESET_MCU_PIN, OUTPUT);
    digitalWrite(RESET_MCU_PIN, LOW);  // low = reset
    delayMicroseconds(100);               // Wait time >100us after external Reset
    digitalWrite(RESET_MCU_PIN, HIGH); // high = normal
  #endif

  Wire.begin();
  delay(40);               // Wait time >40ms after VDD stable

  Wire.begin();
  delay(40);               // Wait time >40ms After VDD stable

  // finally, set # lines, font size, etc.
  normalFunctionSet();

  extendFunctionSet();
  command(LCD_EX_SETBIASOSC | LCD_BIAS_1_5 | LCD_OSC_183HZ);            // 1/5bias, OSC=183Hz@3.0V
  command(LCD_EX_FOLLOWERCONTROL | LCD_FOLLOWER_ON | LCD_RAB_2_00);     // internal follower circuit is turn on
  delay(200);                                       // Wait time >200ms (for power stable)
  normalFunctionSet();

  // turn the display on with no cursor or blinking default
  _displaycontrol = 0x00;//LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  setDisplayControl(LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);

  // clear it off
  clear();

  // Initialize to default text direction (for roman languages)
  _displaymode = 0x00;
  setEntryMode(LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
}

void ST7032DAB::setContrast(uint8_t cont)
{
  extendFunctionSet();
  command(LCD_EX_CONTRASTSETL | (cont & 0x0f));                     // Contrast set
  command(LCD_EX_POWICONCONTRASTH | LCD_ICON_ON | LCD_BOOST_ON | ((cont >> 4) & 0x03)); // Power, ICON, Contrast control
  normalFunctionSet();
}

void ST7032DAB::setContrast5V(uint8_t cont)
{
  extendFunctionSet();
  command(LCD_EX_CONTRASTSETL | (cont & 0x0f));                     // Contrast set
  command(LCD_EX_POWICONCONTRASTH | LCD_ICON_ON | LCD_BOOST_OFF | ((cont >> 4) & 0x03)); // Power, ICON, Contrast control
  normalFunctionSet();
}

void ST7032DAB::setIcon(uint8_t addr, uint8_t bits) {
  uint8_t new_bits = icon_addr_bits[addr] | bits;

  if (new_bits != icon_addr_bits[addr]) {
    icon_addr_bits[addr] = new_bits;
    extendFunctionSet();
    command(LCD_EX_SETICONRAMADDR | (addr & 0x0f));
    write(icon_addr_bits[addr]);
    normalFunctionSet();
  }
}

void ST7032DAB::unsetIcon(uint8_t addr, uint8_t bits) {
  uint8_t new_bits = icon_addr_bits[addr] - (icon_addr_bits[addr] & bits);

  if (new_bits != icon_addr_bits[addr]) {
    icon_addr_bits[addr] = new_bits;
    extendFunctionSet();
    command(LCD_EX_SETICONRAMADDR | (addr & 0x0f));
    write(icon_addr_bits[addr]);
    normalFunctionSet();
  }
}

void ST7032DAB::hideVolume() {
  unsetIcon(ICON_ADDR_VOLUME_1, arrayBits(volume_bars_1));
  unsetIcon(ICON_ADDR_VOLUME_2, arrayBits(volume_bars_2));
  unsetIcon(ICON_ADDR_VOLUME_1, ICON_BIT_VOLUME_1_MUTED_SPK);
}

void ST7032DAB::setVolume(uint8_t volume) {
  uint8_t bits = 0;
  uint8_t i;

  if (currentVolume != volume) {
    currentVolume = volume;

    if (volume > 0) {
      for (i=0; (i+1) <= volume && (i+1) < 4; i++) {
        bits += volume_bars_1[i];
      }

      setIcon(ICON_ADDR_VOLUME_1, bits);
      unsetIcon(ICON_ADDR_VOLUME_1, ~bits);
    } else {
      unsetIcon(ICON_ADDR_VOLUME_1, arrayBits(volume_bars_1));
    }

    bits = 0;

    if (volume > 3) {
      for (i=3; (i+1) <= volume && i < 6; i++) {
        bits += volume_bars_2[i-3];
      }

      setIcon(ICON_ADDR_VOLUME_2, bits);
      unsetIcon(ICON_ADDR_VOLUME_2, ~bits);
    } else {
      unsetIcon(ICON_ADDR_VOLUME_2, arrayBits(volume_bars_2));
    }

  }
}

void ST7032DAB::setVolumePercent(double volumePc) {
  volumePc = constrain(volumePc, 0, 100);

  if(volumePc == 0) {
    setIcon(ICON_ADDR_VOLUME_1, ICON_BIT_VOLUME_1_MUTED_SPK);
  } else {
    setIcon(ICON_ADDR_VOLUME_1, ICON_BIT_VOLUME_1_SPEAKER);
  }

  uint8_t bars = ceil(volumePc / 16.66);
  setVolume(min(bars, 6));
}

void ST7032DAB::initClockDigits() {
  setIcon(0x07, 0x01); // Colon
  setIcon(0x06, 0x18); // 0
  setIcon(0x05, 0x1B); // 0
  setIcon(0x06, 0x06); // 0
  setIcon(0x07, 0x1e); // 0
  setIcon(0x08, 0x1b); // 0
  setIcon(0x09, 0x18); // 0
  setIcon(0x09, 0x06); // 0
  setIcon(0x0a, 0x1e); // 0
}

void ST7032DAB::hideClockDigits() {
  unsetIcon(0x07, 0x01); // Colon
  unsetIcon(0x06, 0x18); // 0
  unsetIcon(0x05, 0x1B); // 0
  unsetIcon(0x06, 0x06); // 0
  unsetIcon(0x07, 0x1e); // 0
  unsetIcon(0x08, 0x1b); // 0
  unsetIcon(0x09, 0x18); // 0
  unsetIcon(0x09, 0x06); // 0
  unsetIcon(0x0a, 0x1e); // 0
}

void ST7032DAB::flashTimeColon() {
  if (((millis() / 1000) % 2) == 0) {
    if(timeColonState == 0) {
      timeColonState = 1;
      setIcon(0x07, 0x01);
    }
  } else {
    if(timeColonState == 1) {
      timeColonState = 0;
      unsetIcon(0x07, 0x01);
    }
  }
}

void ST7032DAB::flashAlarmIcon() {
  if (((millis() / 1000) % 2) == 0) {
    if(alarmIconState == 0) {
      setIcon(ICON_ADDR_ALARM, ICON_BIT_ALARM_BELL);
      alarmIconState = 1;
    }
  } else {
    if(alarmIconState == 1) {
      unsetIcon(ICON_ADDR_ALARM, ICON_BIT_ALARM_BELL);
      alarmIconState = 0;
    }
  }
}

void ST7032DAB::setFirstDigit(uint8_t digit) {
  digit = constrain(digit, 0, 9);

  if (firstDigit != digit) {
    firstDigit = digit;
    unsetIcon(0x06, 0x18);
    unsetIcon(0x05, 0x1F);

    switch (digit) {
      case 0:
        setIcon(0x06, 0x18);
        setIcon(0x05, 0x1B);
        break;
      case 1:
        setIcon(0x06, 0x18);
        break;
      case 2:
        setIcon(0x06, 0x08);
        setIcon(0x05, 0x17);
        break;
      case 3:
        setIcon(0x06, 0x18);
        setIcon(0x05, 0x15);
        break;
      case 4:
        setIcon(0x06, 0x18);
        setIcon(0x05, 0x0C);
        break;
      case 5:
        setIcon(0x06, 0x10);
        setIcon(0x05, 0x1D);
        break;
      case 6:
        setIcon(0x06, 0x10);
        setIcon(0x05, 0x1F);
        break;
      case 7:
        setIcon(0x06, 0x18);
        setIcon(0x05, 0x10);
        break;
      case 8:
        setIcon(0x06, 0x18);
        setIcon(0x05, 0x1F);
        break;
      case 9:
        setIcon(0x06, 0x18);
        setIcon(0x05, 0x1C);
        break;
    }
  }
}

void ST7032DAB::setSecondDigit(uint8_t digit) {
  digit = constrain(digit, 0, 9);

  if (secondDigit != digit) {
    secondDigit = digit;
    unsetIcon(0x06, 0x07);
    unsetIcon(0x07, 0x1e);

    switch (digit) {
      case 0:
        setIcon(0x06, 0x06);
        setIcon(0x07, 0x1e);
        break;
      case 1:
        setIcon(0x07, 0x06);
        break;
      case 2:
        setIcon(0x06, 0x05);
        setIcon(0x07, 0x1a);
        break;
      case 3:
        setIcon(0x06, 0x05);
        setIcon(0x07, 0x0e);
        break;
      case 4:
        setIcon(0x06, 0x03);
        setIcon(0x07, 0x06);
        break;
      case 5:
        setIcon(0x06, 0x07);
        setIcon(0x07, 0x0c);
        break;
      case 6:
        setIcon(0x06, 0x07);
        setIcon(0x07, 0x1c);
        break;
      case 7:
        setIcon(0x06, 0x04);
        setIcon(0x07, 0x06);
        break;
      case 8:
        setIcon(0x06, 0x07);
        setIcon(0x07, 0x1e);
        break;
      case 9:
        setIcon(0x06, 0x07);
        setIcon(0x07, 0x06);
        break;
    }
  }
}

void ST7032DAB::setThirdDigit(uint8_t digit) {
  digit = constrain(digit, 0, 9);

  if (thirdDigit != digit) {
    thirdDigit = digit;
    unsetIcon(0x08, 0x1f);
    unsetIcon(0x09, 0x18);

    switch (digit) {
      case 0:
        setIcon(0x08, 0x1b);
        setIcon(0x09, 0x18);
        break;
      case 1:
        setIcon(0x09, 0x18);
        break;
      case 2:
        setIcon(0x08, 0x17);
        setIcon(0x09, 0x08);
        break;
      case 3:
        setIcon(0x08, 0x15);
        setIcon(0x09, 0x18);
        break;
      case 4:
        setIcon(0x08, 0x0c);
        setIcon(0x09, 0x18);
        break;
      case 5:
        setIcon(0x08, 29);
        setIcon(0x09, 16);
        break;
      case 6:
        setIcon(0x08, 31);
        setIcon(0x09, 16);
        break;
      case 7:
        setIcon(0x08, 16);
        setIcon(0x09, 24);
        break;
      case 8:
        setIcon(0x08, 0x1f);
        setIcon(0x09, 0x18);
        break;
      case 9:
        setIcon(0x08, 0x1c);
        setIcon(0x09, 0x18);
        break;
    }
  }
}

void ST7032DAB::setFourthDigit(uint8_t digit) {
  digit = constrain(digit, 0, 9);

  if (fourthDigit != digit) {
    fourthDigit = digit;
    unsetIcon(0x09, 0x07);
    unsetIcon(0x0a, 0x1e);

    switch (digit) {
      case 0:
        setIcon(0x09, 0x06);
        setIcon(0x0a, 0x1e);
        break;
      case 1:
        setIcon(0x0a, 0x06);
        break;
      case 2:
        setIcon(0x09, 0x05);
        setIcon(0x0a, 0x1a);
        break;
      case 3:
        setIcon(0x09, 0x05);
        setIcon(0x0a, 0x0e);
        break;
      case 4:
        setIcon(0x09, 0x03);
        setIcon(0x0a, 0x06);
        break;
      case 5:
        setIcon(0x09, 0x07);
        setIcon(0x0a, 0x0c);
        break;
      case 6:
        setIcon(0x09, 0x07);
        setIcon(0x0a, 0x1c);
        break;
      case 7:
        setIcon(0x09, 0x04);
        setIcon(0x0a, 0x06);
        break;
      case 8:
        setIcon(0x09, 0x07);
        setIcon(0x0a, 0x1e);
        break;
      case 9:
        setIcon(0x09, 0x07);
        setIcon(0x0a, 0x06);
        break;
    }
  }
}

void ST7032DAB::writeTime(int hours, int minutes) {
  char t[4];
  hours = constrain(hours, 0, 23);
  minutes = constrain(minutes, 0, 59);
  sprintf (t, "%02u%02u", hours, minutes); // Add zero padding

  if (!char_cmp(t, currentDisplayedTime)) {

    for ( int i = 0 ; i < 4 ; ++i ) {
      currentDisplayedTime[i] = t[i];
    }

    setFirstDigit(t[0] - '0');
    setSecondDigit(t[1] - '0');
    setThirdDigit(t[2] - '0');
    setFourthDigit(t[3] - '0');
  }
}

void ST7032DAB::showBattery() {
  setIcon(ICON_ADDR_BATTERY, ICON_BIT_BATTERY);
}

void ST7032DAB::hideBattery() {
  unsetIcon(ICON_ADDR_BATTERY, ICON_BIT_BATTERY);
  unsetIcon(ICON_ADDR_BATTERY, arrayBits(battery_bars));
}

void ST7032DAB::setBattery(uint8_t segments) {
  uint8_t bits = ICON_BIT_BATTERY;

  if (currentBatteryStrength != segments) {
    currentBatteryStrength = segments;
    if (segments > 0) {
      uint8_t i;
      for (i = 1; i <= segments; i++) {
        bits += battery_bars[i - 1];
      }
      unsetIcon(ICON_ADDR_BATTERY, ~bits);
    } else {
      unsetIcon(ICON_ADDR_BATTERY, arrayBits(battery_bars));
    }

    setIcon(ICON_ADDR_BATTERY, bits);
  }
}

void ST7032DAB::setBatteryPercent(double batteryPc) {
  batteryPc = constrain(batteryPc, 0, 100);
  uint8_t bars = ceil(batteryPc / 12.5);
  setBattery(ceil(bars / 2));
}

void ST7032DAB::hideSignal() {
  unsetIcon(ICON_ADDR_SIGNAL_1, ICON_BIT_SIGNAL_1_BAR_1);
  unsetIcon(ICON_ADDR_SIGNAL_2, arrayBits(strength_bars_2));
}

void ST7032DAB::showSignal() {
  setIcon(ICON_ADDR_SIGNAL_1, ICON_BIT_SIGNAL_1_ANT);
}

void ST7032DAB::setSignalPercent(double strengthPc) {
  strengthPc = constrain(strengthPc, 0, 100);
  uint8_t bars = ceil(strengthPc / 16.66);
  setSignal(min(bars, 6));
}

void ST7032DAB::setSignal(uint8_t strength) {
  uint8_t bits = ICON_BIT_SIGNAL_1_ANT;

  if (currentSignalStrength != strength) {
    currentSignalStrength = strength;

    if (strength > 0) {
      bits |= ICON_BIT_SIGNAL_1_BAR_1;
    } else {
      unsetIcon(ICON_ADDR_SIGNAL_1, ICON_BIT_SIGNAL_1_BAR_1);
    }

    setIcon(ICON_ADDR_SIGNAL_1, bits);
    bits = 0;

    if (strength > 1) {
      uint8_t i;
      for (i = 2; i <= strength; i++) {
        bits += strength_bars_2[i - 2];
      }
      setIcon(ICON_ADDR_SIGNAL_2, bits);
      unsetIcon(ICON_ADDR_SIGNAL_2, ~bits);
    } else {
      unsetIcon(ICON_ADDR_SIGNAL_2, arrayBits(strength_bars_2));
    }
  }
}

/********** high level commands, for the user! */
void ST7032DAB::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void ST7032DAB::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void ST7032DAB::setCursor(uint8_t col, uint8_t row)
{
  const int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  if ( row > _numlines ) {
    row = _numlines - 1;  // we count rows starting w/0
  }

  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void ST7032DAB::noDisplay() {
  resetDisplayControl(LCD_DISPLAYON);
}
void ST7032DAB::display() {
  setDisplayControl(LCD_DISPLAYON);
}

// Turns the underline cursor on/off
void ST7032DAB::noCursor() {
  resetDisplayControl(LCD_CURSORON);
}
void ST7032DAB::cursor() {
  setDisplayControl(LCD_CURSORON);
}

// Turn on and off the blinking cursor
void ST7032DAB::noBlink() {
  resetDisplayControl(LCD_BLINKON);
}
void ST7032DAB::blink() {
  setDisplayControl(LCD_BLINKON);
}

// These commands scroll the display without changing the RAM
void ST7032DAB::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void ST7032DAB::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void ST7032DAB::leftToRight(void) {
  setEntryMode(LCD_ENTRYLEFT);
}

// This is for text that flows Right to Left
void ST7032DAB::rightToLeft(void) {
  resetEntryMode(LCD_ENTRYLEFT);
}

// This will 'right justify' text from the cursor
void ST7032DAB::autoscroll(void) {
  setEntryMode(LCD_ENTRYSHIFTINCREMENT);
}

// This will 'left justify' text from the cursor
void ST7032DAB::noAutoscroll(void) {
  resetEntryMode(LCD_ENTRYSHIFTINCREMENT);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void ST7032DAB::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(charmap[i]);
  }
}

uint8_t ST7032DAB::arrayBits(int* arr) {
  uint8_t rtn = 0;
  int i;

  for (i = 0; i <= sizeof(arr); i++)
  {
    rtn |= arr[i];
  }

  return rtn;
}

bool ST7032DAB::char_cmp(char *a, char *b) {
  int n;

  // if their lengths are different, return false
  if (sizeof(a) != sizeof(b)) return false;

  // test each element to be the same. if not, return false
  for (n = 0; n < sizeof(a); n++) if (a[n] != b[n]) return false;

  //ok, if we have not returned yet, they are equal :)
  return true;
}

void ST7032DAB::millisToTime(unsigned long t, int his[])
{
  t = t/1000;
  his[0] = t / 3600;
  t = t % 3600;
  his[1] = t / 60;
  his[2] = t % 60;
}

char * ST7032DAB::millisToString(unsigned long t) {
  int his[3];
  millisToTime(t, his);
  static char str[8];
  sprintf(str, "%02d:%02d:%02d", his[0], his[1], his[2]);
  return str;
}

/*********** mid level commands, for sending data/cmds */

void ST7032DAB::command(uint8_t value) {
  Wire.beginTransmission(_i2c_addr);
  Wire.write((uint8_t)0x00);
  Wire.write(value);
  Wire.endTransmission();
  delayMicroseconds(27);    // >26.3us
}

size_t ST7032DAB::write(uint8_t value) {
  Wire.beginTransmission(_i2c_addr);
  Wire.write((uint8_t)0x40);
  Wire.write(value);
  Wire.endTransmission();
  delayMicroseconds(27);    // >26.3us

  return 1;
}
