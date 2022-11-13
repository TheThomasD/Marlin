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
 * lcd/extui/geeetech_a30t/geeetech_a30t_receive.cpp
 *
 * Receive commands from display
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
    UiCommand TouchDisplay::receiveCommand()
    {
        return parseCommandString(receiveCommandString());
    }

    String TouchDisplay::receiveCommandString()
    {
        String receivedLine = LCD_SERIAL.readStringUntil('\n');
        int lastStarChar = receivedLine.lastIndexOf('*');

        // just remove checksum without checking 8-0
        String result = receivedLine;
        if (lastStarChar > 0)
            result = receivedLine.substring(0, lastStarChar);

        // have to do "while" here because at least one command uses two times "N-0" :-(
        while (result.startsWith("N-0 "))
            result = result.substring(4);

        return result;
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT