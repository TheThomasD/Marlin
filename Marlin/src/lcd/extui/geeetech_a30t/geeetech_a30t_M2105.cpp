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
 * lcd/extui/geeetech_a30t/geeetech_a30t_M2105.cpp
 *
 * This file implements the M2105 command to control extruder movement
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
    millis_t TouchDisplay::nextExtrude = 0;
    millis_t TouchDisplay::nextExtruderSwitch = 0;
    bool TouchDisplay::extrude = false;
    EMode TouchDisplay::extrudeMode = LOAD;
    uint8_t TouchDisplay::currentCleaningExtruder = E0;

    void TouchDisplay::handle_M2105_ExtruderCommands(const UiCommand &command)
    {
        switch (command.parameters[S].charAt(0))
        {
        case '2':
            handle_M2105_LoadUnloadClean(LOAD);
            break;
        case '3':
            handle_M2105_LoadUnloadClean(UNLOAD);
            break;
        case '5':
            handle_M2105_LoadUnloadClean(CLEAN);
            break;
        case '4':
        default:
            handle_M2105_S4_StopMotors();
            break;
        }
    }

    void TouchDisplay::handle_M2105_LoadUnloadClean(const EMode &extrudeMode)
    {
        TouchDisplay::extrudeMode = extrudeMode;
        nextExtrude = 0;
        extrude = true;
        if (CLEAN == extrudeMode)
            currentCleaningExtruder = E0;
    }

    void TouchDisplay::handle_M2105_S4_StopMotors()
    {
        extrude = false;
    }

    void TouchDisplay::extrudeIfNeeded(const millis_t &currentTimeMs)
    {
        if (extrude)
        {
            if (CLEAN == extrudeMode && ELAPSED(currentTimeMs, nextExtruderSwitch))
            {
                nextExtruderSwitch = currentTimeMs + SWITCH_EXTRUDER_CYCLE_IN_MS;
                mixer.set_collector(0, E0 == currentCleaningExtruder ? 1.0 : 0.0);
                mixer.set_collector(1, E1 == currentCleaningExtruder ? 1.0 : 0.0);
                mixer.set_collector(2, E2 == currentCleaningExtruder ? 1.0 : 0.0);
                mixer.normalize();

                if (E0 == currentCleaningExtruder)
                    currentCleaningExtruder = E1;
                else if (E1 == currentCleaningExtruder)
                    currentCleaningExtruder = E2;
                else
                    currentCleaningExtruder = E0;
            }
            delay(20);
            if (ELAPSED(currentTimeMs, nextExtrude))
            {
                nextExtrude = currentTimeMs + EXTRUDE_CYCLE_IN_MS;
                unscaled_e_move(UNLOAD == extrudeMode ? -1.0 : 1.0, 1.0);
            }
        }
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT