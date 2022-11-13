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
 * This file implements the M2135 command for setting color mix
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
    void TouchDisplay::handle_M2135_SetColorMix(const UiCommand &command)
    {
        uintmax_t value = strtoumax(command.parameters[P].c_str(), NULL, 10);

        mixer.collector[0] = ((uint8_t)value) & COLOR_MASK;
        mixer.collector[1] = ((uint8_t)(value >> 8)) & COLOR_MASK;
        mixer.collector[2] = (uint8_t)(value >> 16);
        mixer.normalize();
    }
} // namespace Geeetech

#endif // GEEETECH_A30T_TFT