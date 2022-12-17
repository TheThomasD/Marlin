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
 * Geeetech GTM32 103 V1 board pin assignments
 */

#include "env_validate.h"

#define BOARD_INFO_NAME      "GTM32 103 V1"
#define DEFAULT_MACHINE_NAME "STM32F103VET6"

#define BOARD_NO_NATIVE_USB

// Set correct timers (will lead to conflicts and instabilities otherwise)
#define TEMP_TIMER  1
#define STEP_TIMER  3

//#define DISABLE_DEBUG

//
// It is required to disable JTAG function because its pins are
// used as GPIO to drive the Y axis stepper.
// DO NOT ENABLE!
//
#define DISABLE_JTAG

//
// If you don't need the SWDIO functionality (any more), you may
// disable SWD here to regain PA13/PA14 pins for other use.
//
//#define DISABLE_JTAGSWD

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

// Enable EEPROM Emulation for this board as it doesn't have EEPROM
#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PA15
//#define X_MAX_PIN                           PC6
#define Y_MIN_PIN                           PD4
#define Y_MAX_PIN                           PD11
#define Z_MIN_PIN                           PD10
//#define Z_MAX_PIN                           PB3
#define Z2_MIN_PIN                          PB7

//
// Steppers
//
#define X_STEP_PIN                          PA11
#define X_DIR_PIN                           PA12
#define X_ENABLE_PIN                        PA8

#define Y_STEP_PIN                          PD15
#define Y_DIR_PIN                           PD3
#define Y_ENABLE_PIN                        PD14

#define Z_STEP_PIN                          PD6
#define Z_DIR_PIN                           PD7
#define Z_ENABLE_PIN                        PD5

#define Z2_STEP_PIN                         PB5
#define Z2_DIR_PIN                          PB6
#define Z2_ENABLE_PIN                       PB4

// Extruder stepper pins
// NOTE: Numbering here is made according to EXT connector numbers,
//       the FANx_PWM line numbering in the schematics is reverse.
//       That is, E0_*_PIN are the E2_* lines connected to E2_A1 step
//       stick that drives the EXT0 output on the board.
//
#define E0_STEP_PIN                         PB9
#define E0_DIR_PIN                          PE0
#define E0_ENABLE_PIN                       PB8

#define E1_STEP_PIN                         PE4
#define E1_DIR_PIN                          PE5
#define E1_ENABLE_PIN                       PE3

#define E2_STEP_PIN                         PC14
#define E2_DIR_PIN                          PC15
#define E2_ENABLE_PIN                       PC13

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PE9   // EXT0 port
//#define HEATER_1_PIN                        PB5   // EXT1 port
//#define HEATER_2_PIN                        PB4   // EXT2 port
#define HEATER_BED_PIN                      PE14   // CON2X3 hotbed port

//
// These are FAN PWM pins on EXT0..EXT2 connectors.
//
#define FAN_PIN                             PA6   // EXT0 port
//#define FAN1_PIN                            PB8   // EXT1 port
//#define FAN2_PIN                            PB7   // EXT2 port

#ifndef E0_AUTO_FAN_PIN
  //#define E0_AUTO_FAN_PIN                   PA6   // EXT0 port, used as main extruder fan
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0   // EXT0 port
//#define TEMP_1_PIN                          PC1   // EXT1 port
//#define TEMP_2_PIN                          PC2   // EXT2 port
#define TEMP_BED_PIN                        PC3   // CON2X3 hotbed port

//
// Misc. Functions
//
//#define LED_PWM                             PD12  // External LED, pin 2 on LED labeled connector

//
// LCD / Controller
//
#define LCD_UART_TX                   PD8
#define LCD_UART_RX                   PD9

//
// Beeper
//
/*
#ifdef GTM32_PRO_VB_USE_LCD_BEEPER
  // This is pin 32 on J2 FFC40 and pin, goes to the beeper
  // on Geeetech's version of RepRapDiscount Smart Controller
  // (e.g. on Rostock 301)
  #define BEEPER_PIN                        PE12
#else
  // This is the beeper on the board itself
  #define BEEPER_PIN                        PB10
#endif
*/

#define SDIO_SUPPORT
#define SD_DETECT_PIN                     PC7

/**
 * The on-board TF_CARD_SOCKET microSD card socket has no SD Detect pin wired.
 *
 * The FFC10 (SD_CARD) connector has the same pins as those routed to the FFC40 (J2)
 * connector, which usually go to the SD Card slot on the Geeetech version of the
 * RepRapDiscount Smart Controller. Both connectors have the card detect signal.
 *
 * The on-board SD card and the external card (on either SD_CARD or J2) are two
 * separate devices and can work simultaneously. Unfortunately, Marlin only supports
 * a single SPI Flash device (as of 2019-07-05) so only one is enabled here.
 */
#define GTM32_PRO_VB_USE_EXT_SDCARD
#if ENABLED(GTM32_PRO_VB_USE_EXT_SDCARD)
  //
  // SD Card on RepRapDiscount Smart Controller (J2) or on SD_CARD connector
  //
  #define SD_SS_PIN                         PC11
  #define SD_SCK_PIN                        PC12
  #define SD_MOSI_PIN                       PD2
  #define SD_MISO_PIN                       PC8
  #define SD_DETECT_PIN                     PC7
#else
  //
  // Use the on-board card socket labeled TF_CARD_SOCKET
  //
//  #define SD_SS_PIN                         PA4
//  #define SD_SCK_PIN                        PA5
//  #define SD_MOSI_PIN                       PA7
//  #define SD_MISO_PIN                       PA6
//  #define SD_DETECT_PIN                     -1    // Card detect is not connected
#endif

//#define SDSS                           SD_SS_PIN

//
// ESP WiFi can be soldered to J9 connector which is wired to USART2.
// Must define WIFISUPPORT in Configuration.h for the printer.
//
//#define ESP_WIFI_MODULE_COM                    2
//#define ESP_WIFI_MODULE_BAUDRATE          115200
//#define ESP_WIFI_MODULE_RESET_PIN           -1