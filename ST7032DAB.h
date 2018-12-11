#ifndef __ST7032DAB_H__
#define __ST7032DAB_H__

#include <stdint.h>
#include <Print.h>

#define ST7032DAB_I2C_DEFAULT_ADDR   0x3E
#define RESET_MCU_PIN                5

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

#define LCD_EX_SETBIASOSC       0x10        // Bias selection / Internal OSC frequency adjust
#define LCD_EX_SETICONRAMADDR   0x40        // Set ICON RAM address
#define LCD_EX_POWICONCONTRASTH 0x50        // Power / ICON control / Contrast set(high byte)
#define LCD_EX_FOLLOWERCONTROL  0x60        // Follower control
#define LCD_EX_CONTRASTSETL     0x70        // Contrast set(low byte)

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5X10DOTS            0x04
#define LCD_5X8DOTS             0x00
#define LCD_EX_INSTRUCTION      0x01        // IS: instruction table select

// flags for Bias selection
#define LCD_BIAS_1_4            0x08        // bias will be 1/4
#define LCD_BIAS_1_5            0x00        // bias will be 1/5

// flags Power / ICON control / Contrast set(high byte)
#define LCD_ICON_ON             0x08        // ICON display on
#define LCD_ICON_OFF            0x00        // ICON display off
#define LCD_BOOST_ON            0x04        // booster circuit is turn on
#define LCD_BOOST_OFF           0x00        // booster circuit is turn off
#define LCD_OSC_122HZ           0x00        // 122Hz@3.0V
#define LCD_OSC_131HZ           0x01        // 131Hz@3.0V
#define LCD_OSC_144HZ           0x02        // 144Hz@3.0V
#define LCD_OSC_161HZ           0x03        // 161Hz@3.0V
#define LCD_OSC_183HZ           0x04        // 183Hz@3.0V
#define LCD_OSC_221HZ           0x05        // 221Hz@3.0V
#define LCD_OSC_274HZ           0x06        // 274Hz@3.0V
#define LCD_OSC_347HZ           0x07        // 347Hz@3.0V

// flags Follower control
#define LCD_FOLLOWER_ON         0x08        // internal follower circuit is turn on
#define LCD_FOLLOWER_OFF        0x00        // internal follower circuit is turn off
#define LCD_RAB_1_00            0x00        // 1+(Rb/Ra)=1.00
#define LCD_RAB_1_25            0x01        // 1+(Rb/Ra)=1.25
#define LCD_RAB_1_50            0x02        // 1+(Rb/Ra)=1.50
#define LCD_RAB_1_80            0x03        // 1+(Rb/Ra)=1.80
#define LCD_RAB_2_00            0x04        // 1+(Rb/Ra)=2.00
#define LCD_RAB_2_50            0x05        // 1+(Rb/Ra)=2.50
#define LCD_RAB_3_00            0x06        // 1+(Rb/Ra)=3.00
#define LCD_RAB_3_75            0x07        // 1+(Rb/Ra)=3.75

#define DEFAULT_CONTRAST        30

// ICON definitions
#define ICON_ADDR_VOLUME_1        0x00

#define ICON_BIT_VOLUME_1_SPEAKER   0x08
#define ICON_BIT_VOLUME_1_MUTE_X    0x10
#define ICON_BIT_VOLUME_1_MUTED_SPK 0x18
#define ICON_BIT_VOLUME_1_BAR_1   0x04
#define ICON_BIT_VOLUME_1_BAR_2   0x02
#define ICON_BIT_VOLUME_1_BAR_3   0x01

#define ICON_ADDR_VOLUME_2        0x01
#define ICON_BIT_VOLUME_2_EQ      0x01
#define ICON_BIT_VOLUME_2_ST      0x02
#define ICON_BIT_VOLUME_2_BAR_4   0x10
#define ICON_BIT_VOLUME_2_BAR_5   0x08
#define ICON_BIT_VOLUME_2_BAR_6   0x04

#define ICON_ADDR_BATTERY         0x03
#define ICON_BIT_BATTERY          0x10
#define ICON_BIT_BATTERY_1        0x08
#define ICON_BIT_BATTERY_2        0x04
#define ICON_BIT_BATTERY_3        0x02
#define ICON_BIT_BATTERY_4        0x01

#define ICON_ADDR_CLOCK_ICON      0x04
#define ICON_BIT_CLOCK            0x02
#define ICON_BIT_DAB              0x04
#define ICON_BIT_FM               0x08
#define ICON_BIT_SD               0x10

#define ICON_ADDR_ALARM           0x0B
#define ICON_BIT_ALARM_1          0x04
#define ICON_BIT_ALARM_2          0x08
#define ICON_BIT_ALARM_BELL       0x10

#define ICON_ADDR_PLAYBACK_1        0x0C
#define ICON_BIT_PLAYBACK_1_REC     0x01
#define ICON_BIT_PLAYBACK_1_BACK_1  0x02
#define ICON_BIT_PLAYBACK_1_BACK_2  0x04

#define ICON_ADDR_PLAYBACK_2      0x0D
#define ICON_BIT_PLAYBACK_2_FWD_1 0x01
#define ICON_BIT_PLAYBACK_2_PAUSE 0x02
#define ICON_BIT_PLAYBACK_2_PLAY  0x08

#define ICON_ADDR_PLAYBACK_3      0x0E
#define ICON_BIT_PLAYBACK_3_REVU  0x08
#define ICON_BIT_PLAYBACK_3_FWD_2 0x10

#define ICON_ADDR_SIGNAL_1        0x0E
#define ICON_BIT_SIGNAL_1_ANT     0x02
#define ICON_BIT_SIGNAL_1_BAR_1   0x01

#define ICON_ADDR_SIGNAL_2        0x0F
#define ICON_BIT_SIGNAL_2_BAR_2   0x10
#define ICON_BIT_SIGNAL_2_BAR_3   0x08
#define ICON_BIT_SIGNAL_2_BAR_4   0x04
#define ICON_BIT_SIGNAL_2_BAR_5   0x02
#define ICON_BIT_SIGNAL_2_BAR_6   0x01

class ST7032DAB : public Print {
  public:
    ST7032DAB(int i2c_addr = ST7032DAB_I2C_DEFAULT_ADDR);

    void init(uint8_t contrast);
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5X8DOTS);

    void setContrast(uint8_t cont = DEFAULT_CONTRAST);
	void setContrast5V(uint8_t cont = DEFAULT_CONTRAST);
    void setIcon(uint8_t addr, uint8_t bit);
    void unsetIcon(uint8_t addr, uint8_t bit);
    void setVolume(uint8_t volume);
    void setVolumePercent(double volumePc);
    void hideVolume();
    void setSignal(uint8_t strength);
    void setSignalPercent(double strengthPc);
    void setBattery(uint8_t segments);
    void setBatteryPercent(double batteryPc);
    void initClockDigits();
    void hideClockDigits();
    void setFirstDigit(uint8_t digit);
    void setSecondDigit(uint8_t digit);
    void setThirdDigit(uint8_t digit);
    void setFourthDigit(uint8_t digit);
    void writeTime(int hours, int minutes);
    void flashTimeColon();
    void flashAlarmIcon();
    void showBattery();
    void hideBattery();
    void showSignal();
    void hideSignal();
    void clear();
    void home();
    uint8_t arrayBits(int* arr);
    bool char_cmp(char *a, char *b);
    void millisToTime(unsigned long t, int his[]);
    char * millisToString(unsigned long t);

    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();

    void createChar(uint8_t location, uint8_t charmap[]);
    void setCursor(uint8_t col, uint8_t row);
    virtual size_t write(uint8_t value);
    void command(uint8_t value);

  private:
    void setDisplayControl(uint8_t setBit);
    void resetDisplayControl(uint8_t resetBit);
    void setEntryMode(uint8_t setBit);
    void resetEntryMode(uint8_t resetBit);
    void normalFunctionSet();
    void extendFunctionSet();
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _initialized;
    uint8_t _numlines;
    uint8_t _currline;
    uint8_t _i2c_addr;
};

#endif  // __ST7032DAB_H__
