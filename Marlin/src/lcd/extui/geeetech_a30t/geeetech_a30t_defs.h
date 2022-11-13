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
 * lcd/extui/geeetech_a30t/geeetech_a30t_defs.h
 */

// settings
#define GEEETECH_DISPLAY_DEBUG // comment out for production
#define SEND_CYCLE_IN_MS 1000
#define EXTRUDE_CYCLE_IN_MS 1000
#define SWITCH_EXTRUDER_CYCLE_IN_MS 5000

// enum, string array, count, boolean generation
#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_COUNT(ENUM) +1
#define GENERATE_BOOL_COMPARE(PARAMETER, COMMAND) || PARAMETER == COMMAND

// command enum
#define FOREACH_COMMAND(COMMAND) \
    COMMAND(Unknown)             \
    COMMAND(GCode)               \
    COMMAND(L101)                \
    COMMAND(M290)                \
    COMMAND(M2011)               \
    COMMAND(M2105)               \
    COMMAND(M2106)               \
    COMMAND(M2107)               \
    COMMAND(M2120)               \
    COMMAND(M2134)               \
    COMMAND(M2135)               \
    COMMAND(M2140)

// parameter enum
#define FOREACH_PARAMETER(PARAM) \
    PARAM(P)                     \
    PARAM(S)                     \
    PARAM(A)                     \
    PARAM(B)                     \
    PARAM(C)                     \
    PARAM(D)                     \
    PARAM(R)                     \
    PARAM(Z)                     \
    PARAM(FW)

// modes of extruder moves
#define FOREACH_E_MODE(MODE) \
    MODE(LOAD)               \
    MODE(UNLOAD)             \
    MODE(CLEAN)

// commands that need immediate answer
#define FOREACH_ANSWER(PARAMETER, COMMAND) \
    COMMAND(PARAMETER, L101)               \
    COMMAND(PARAMETER, M290)               \
    COMMAND(PARAMETER, M2011)              \
    COMMAND(PARAMETER, M2120)              \
    COMMAND(PARAMETER, M2135)              \
    COMMAND(PARAMETER, M2140)

// manual leveling
#define MANUAL_LEVELING_POINT_MARGIN 42.00
#define MANUAL_LEVELING_MOVE_Z_HOP 5
#define MANUAL_LEVELING_MOVE_DIRECTION_UP true
#define MANUAL_LEVELING_MOVE_DIRECTION_DOWN false
#define MANUAL_LEVELING_MOVE_BIG_STEP true
#define MANUAL_LEVELING_MOVE_SMALL_STEP false
#define MANUAL_LEVELING_POS_RR_X X_MAX_BED - MANUAL_LEVELING_POINT_MARGIN + 5 // bed is actually larger
#define MANUAL_LEVELING_POS_RR_Y Y_MAX_BED - MANUAL_LEVELING_POINT_MARGIN
#define MANUAL_LEVELING_POS_RL_X X_MIN_BED + MANUAL_LEVELING_POINT_MARGIN
#define MANUAL_LEVELING_POS_RL_Y Y_MAX_BED - MANUAL_LEVELING_POINT_MARGIN
#define MANUAL_LEVELING_POS_FL_X X_MIN_BED + MANUAL_LEVELING_POINT_MARGIN
#define MANUAL_LEVELING_POS_FL_Y Y_MIN_BED + MANUAL_LEVELING_POINT_MARGIN
#define MANUAL_LEVELING_POS_FR_X X_MAX_BED - MANUAL_LEVELING_POINT_MARGIN + 5 // bed is actually larger
#define MANUAL_LEVELING_POS_FR_Y Y_MIN_BED + MANUAL_LEVELING_POINT_MARGIN

// status values
#define PRINT_STATUS_IDLE 0
#define PRINT_STATUS_PRINTING 1
#define PRINT_STATUS_PAUSED 2
#define PRINT_STATUS_FINISHED 4

#define FILAMENT_SENSOR_DISABLED 255
#define FILAMENT_SENSOR_OUT 0
#define FILAMENT_SENSOR_IN 1

// status macros
#define FEEDRATE (uint8_t) round(ExtUI::getFeedrate_mm_s())
#define BED_ACTIVE thermalManager.temp_bed.target > 0
#define E0_ACTIVE thermalManager.temp_hotend[0].target > 0
#define SD_ACTIVE isMediaInserted()
#define F0_SPEED (uint8_t) round(getActualFan_percent(FAN0))
#define PRINT_SPEED (uint8_t) round(getFeedrate_percent())
#define FILAMENT_SENSOR_STATUS getFilamentRunoutEnabled()        \
                                   ? (getFilamentRunoutState()   \
                                          ? FILAMENT_SENSOR_IN   \
                                          : FILAMENT_SENSOR_OUT) \
                                   : FILAMENT_SENSOR_DISABLED
#define CURRENT_FILENAME card.filename
#define MOTOR_TENSION_STATUS stepper.axis_is_enabled(X_AXIS) ||     \
                                 stepper.axis_is_enabled(Y_AXIS) || \
                                 stepper.axis_is_enabled(Z_AXIS) || \
                                 stepper.axis_is_enabled(E_AXIS)
