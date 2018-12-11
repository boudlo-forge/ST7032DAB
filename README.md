# ST7032DAB
Arduino Library and examples for CI064-4022-03 i2c LCD Modules designed for DAB Digital Radios. Helper functions are included for setting volume, battery and signal strength by percentage, as well bars. There are also helper functions for setting the 7-segment clock. Most helper functions have been written to minimise unnecessary i2c writes.

The CI064-4022-03 is an application specific 1602 LCD designed for DAB digital radios, as such it has icons for all sorts of useful functions. Notably a signal strength, battery indicator and separate 7-segment clock which can free up valuable character real estate for IoT projects. The i2c interface is provided by an ST7032, and this library is based on the good work of https://github.com/tomozh


3.3V    --+--   VDD

D5      -----   RES

SDA     --*--   SDA

SCL     --*--   SCL

GND     -----   GND

*... 4.7-10k pull-up resistor may be required.

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
