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
 * This file implements the M2011 command for setting double Z offset
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
    void TouchDisplay::handle_M2011_DoubleZOffset(const UiCommand &command)
    {
        if (((String) "0").equals(command.parameters[S]))
        {
            settings.load();
        }
        else
        {
            float_t offsetValue = strtof(command.parameters[S].c_str(), nullptr);
            if ('0' == command.parameters[P].charAt(0)) // Z0 offset
            {
                if (offsetValue > 0)
                    if (endstops.z2_endstop_adj >= 0)
                        endstops.z2_endstop_adj = offsetValue;
                    else
                        endstops.z2_endstop_adj += 0.01;
                else
                    endstops.z2_endstop_adj -= 0.01;
            }
            else // Z1 offset
            {
                if (offsetValue > 0)
                    if (endstops.z2_endstop_adj <= 0)
                        endstops.z2_endstop_adj = -offsetValue;
                    else
                        endstops.z2_endstop_adj -= 0.01;
                else
                    endstops.z2_endstop_adj += 0.01;
            }
        }
    }
} // namespace Geeetech

#endif // GEEETECH_A30T_TFT