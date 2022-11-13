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
 * lcd/extui/geeetech_a30t/geeetech_a30t_handle.cpp
 *
 * Basic command handling
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

    void TouchDisplay::handleGcode(const String &gcode)
    {
#ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Queueing command: ", gcode.c_str());
#endif
        queue.enqueue_now_P(PSTR(gcode.c_str()));
    }

    void TouchDisplay::handleUnkownCommand(const UiCommand &command)
    {
#ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Unknown command ignored: ", command.command.c_str());
#endif
    }

    void TouchDisplay::handleProprietaryCommand(const UiCommand &command)
    {
        switch (command.type)
        {
        case L101: // return from M2107 manual leveling
            simulatedAutoLevelSwitchOn = true;
            disableAxisStatusSend = false;
            break;
        case M290:
            handle_M290_Babystep(command);
            break;
        case M2011:
            handle_M2011_DoubleZOffset(command);
            break;
        case M2105:
            handle_M2105_ExtruderCommands(command);
            break;
        case M2106:
            setFilamentRunoutEnabled('1' == command.parameters[S].charAt(0));
            break;
        case M2107:
            handle_M2107_ManualLeveling(command);
            break;
        case M2120:
            handle_M2120_AutoLeveling(command);
            break;
        case M2134:
            handle_M2134_RequestFirmware(command);
            break;
        case M2135:
            handle_M2135_SetColorMix(command);
            break;
        case M2140:
            handle_M2140_MotionParams(command);
            break;
        default:
#ifdef GEEETECH_DISPLAY_DEBUG
            SERIAL_ECHOLNPGM("Command not handled: ", COMMAND_STRINGS[command.type]);
#endif
            break;
        }
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT