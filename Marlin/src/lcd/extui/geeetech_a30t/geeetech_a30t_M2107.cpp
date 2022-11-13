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

/**
 * lcd/extui/geeetech_a30t/geeetech_a30t_M2134.cpp
 *
 * This file implements the M2107 command for controlling manual leveling
 *  
 * written in 2021 by TheThomasD
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(GEEETECH_A30T_TFT)

#include "geeetech_a30t.h"
#include "../ui_api.h"
#include "../../marlinui.h"

using namespace ExtUI;

namespace Geeetech
{
    void TouchDisplay::handle_M2107_ManualLeveling(const UiCommand &command)
    {
        if (command.parameters[S].equals("10")) // move Z down 0.05mm
            moveUpDownSmallBigStep(MANUAL_LEVELING_MOVE_DIRECTION_UP, MANUAL_LEVELING_MOVE_SMALL_STEP);
        else
            switch (command.parameters[S].charAt(0))
            {
            case '0': // start
                handle_M2107_S0_Start();
                break;
            case '1': // move to pos 1 (RR)
                moveToXYWithZHop(MANUAL_LEVELING_POS_RR_X, MANUAL_LEVELING_POS_RR_Y);
                break;
            case '2': // move to pos 2 (RL)
                moveToXYWithZHop(MANUAL_LEVELING_POS_RL_X, MANUAL_LEVELING_POS_RL_Y);
                break;
            case '3': // move to pos 3 (FL)
                moveToXYWithZHop(MANUAL_LEVELING_POS_FL_X, MANUAL_LEVELING_POS_FL_Y);
                break;
            case '4': // move to pos 4 (FR)
                moveToXYWithZHop(MANUAL_LEVELING_POS_FR_X, MANUAL_LEVELING_POS_FR_Y);
                break;
            case '5': // move to pos 5 (C)
                moveToXYWithZHop(X_CENTER, Y_CENTER);
                break;
            case '6': // move Z up 0.5mm
                moveUpDownSmallBigStep(MANUAL_LEVELING_MOVE_DIRECTION_UP, MANUAL_LEVELING_MOVE_BIG_STEP);
                break;
            case '7': // move Z down 0.5mm
                moveUpDownSmallBigStep(MANUAL_LEVELING_MOVE_DIRECTION_DOWN, MANUAL_LEVELING_MOVE_BIG_STEP);
                break;
            case '8': // OK / save
                handle_M2107_S8_OkSave();
                break;
            case '9': // move Z down 0.05mm
                moveUpDownSmallBigStep(MANUAL_LEVELING_MOVE_DIRECTION_DOWN, MANUAL_LEVELING_MOVE_SMALL_STEP);
                break;
            }
        send_L10_ZOffset();
    }

    void TouchDisplay::handle_M2107_S0_Start()
    {
        disableAxisStatusSend = true;
        send_L10_ZOffset();
        home_if_needed(false);
        do_blocking_move_to_z(0.0 - home_offset.z, 200.0F);
    };

    void TouchDisplay::handle_M2107_S8_OkSave()
    {
        settings.save();
        sendToDisplay("M2107 save success");
    };

    void TouchDisplay::moveToXYWithZHop(const float_t &xPos, const float_t &yPos)
    {
        do_blocking_move_to_z(MANUAL_LEVELING_MOVE_Z_HOP);
        do_blocking_move_to_xy(xPos, yPos);
        do_blocking_move_to_z(0.0 - home_offset.z);
        sendToDisplay("M2107 ok");
    }

    void TouchDisplay::moveUpDownSmallBigStep(const bool &directionUpDown, const bool &bigSmallStep)
    {
        float_t newOffset = home_offset[Z_AXIS];

        if (directionUpDown == MANUAL_LEVELING_MOVE_DIRECTION_UP)
            newOffset -= bigSmallStep == MANUAL_LEVELING_MOVE_BIG_STEP ? 0.5 : 0.05;
        else
            newOffset += bigSmallStep == MANUAL_LEVELING_MOVE_BIG_STEP ? 0.5 : 0.05;

        set_home_offset(Z_AXIS, newOffset);
        do_blocking_move_to_z(0.0 - home_offset.z, 200.0);
    }
} // namespace Geeetech

#endif // GEEETECH_A30T_TFT