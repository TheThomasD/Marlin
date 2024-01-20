/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Geeetech GT2560 Revision A+ board pin assignments
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Geeetech%20GT2560%20Revision%20A+/Hardware_GT2560_RevA+.pdf
 * Origin: https://www.geeetech.com/wiki/images/d/d3/Hardware_GT2560_RevA%2B.pdf
 * ATmega2560
 */

/*
  Pins: https://www.14core.com/wp-content/uploads/2015/06/ATMEGA-2560-Pin-Out-Diagram.png
  Connectors: https://www.geeetech.com/Documents/Users%20Manual%20of%20GT2560.pdf
*/

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME "GT2560 Rev.A+"

//
// Limit Switches
//
#define X_MIN_PIN                             22
#define X_MAX_PIN                             24
#define Y_MIN_PIN                             26
//#define Y_MAX_PIN                             28 // will be used for filament runout sensor
#define Z_MIN_PIN                             30

#define FIL_RUNOUT_PIN 28

#if ENABLED(BLTOUCH)
  #if MB(GT2560_REV_A_PLUS)
    #define SERVO0_PIN                        11
    #define Z_MAX_PIN                         32
  #else
    #define SERVO0_PIN                        32
    #define Z_MAX_PIN                         -1
  #endif
#else
  #define SERVO0_PIN                          11 // will be used for probe
  #define Z_MAX_PIN                           32 // Z2 endstop
#endif

//
// Steppers
//
#define X_STEP_PIN                            25
#define X_DIR_PIN                             23
#define X_ENABLE_PIN                          27

#define Y_STEP_PIN                            31
#define Y_DIR_PIN                             33
#define Y_ENABLE_PIN                          29

#define Z_STEP_PIN                            37
#define Z_DIR_PIN                             39
#define Z_ENABLE_PIN                          35

#define E0_STEP_PIN                           43
#define E0_DIR_PIN                            45
#define E0_ENABLE_PIN                         41

#define Z2_STEP_PIN                           49 // was E1
#define Z2_DIR_PIN                            47 // was E1
#define Z2_ENABLE_PIN                         48 // was E1

//
// Temperature Sensors
//
#define TEMP_0_PIN                             8
#define TEMP_1_PIN                             9
#define TEMP_BED_PIN                          10

//
// Heaters / Fans
//
#define FAN_PIN                                2 // was HEATER_0_PIN
#define HEATER_0_PIN                           3 // was HEATER_1_PIN
#define HEATER_BED_PIN                         4
#ifndef FAN1_PIN
  #define FAN1_PIN                             7 // was FAN_PIN
#endif

//
// Misc. Functions
//
#define SDSS                                  53
//#define LED_PIN                               13 // disabled for timer
#define PS_ON_PIN                             12
#define SUICIDE_PIN                           54  // Must be enabled at startup to keep power flowing
#define KILL_PIN                              -1

// the following is required to allow for the probe to work correctly
#undef TIMER3A
#undef TIMER4A
#define TIMER3A 12
#define TIMER4A 46

#if HAS_WIRED_LCD
  
  #define BEEPER_PIN                          18
  
  #if IS_NEWPANEL
    
    #if ENABLED(MKS_MINI_12864)
      #define DOGLCD_A0                        5
      #define DOGLCD_CS                       21
      #define BTN_EN1                         40
      #define BTN_EN2                         42
    #elif ENABLED(FYSETC_MINI_12864)
      // Disconnect EXP2-1 and EXP2-2, otherwise future firmware upload won't work.
      #define DOGLCD_A0                       20
      #define DOGLCD_CS                       17
      
      #define NEOPIXEL_PIN                    21
      #define BTN_EN1                         42
      #define BTN_EN2                         40
      
      #define LCD_RESET_PIN                   16
      
      #define LCD_CONTRAST_INIT              220
      
      #define LCD_BACKLIGHT_PIN               -1
    #else
      #define LCD_PINS_RS                     20
      #define LCD_PINS_ENABLE                 17
      #define LCD_PINS_D4                     16
      #define LCD_PINS_D5                     21
      #define LCD_PINS_D6                      5
      #define LCD_PINS_D7                      6
      #define BTN_EN1                         42
      #define BTN_EN2                         40
    #endif
    
    #define BTN_ENC                           19
    #define SD_DETECT_PIN                     38
    
  #else                                           // !IS_NEWPANEL
    
    #define SHIFT_CLK_PIN                     38
    #define SHIFT_LD_PIN                      42
    #define SHIFT_OUT_PIN                     40
    #define SHIFT_EN_PIN                      17
    
    #define LCD_PINS_RS                       16
    #define LCD_PINS_ENABLE                    5
    #define LCD_PINS_D4                        6
    #define LCD_PINS_D5                       21
    #define LCD_PINS_D6                       20
    #define LCD_PINS_D7                       19
    
    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif
    
    #define SD_DETECT_PIN                     -1
    
  #endif // !IS_NEWPANEL
  
#endif // HAS_WIRED_LCD

