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
    bool TouchDisplay::receiveCommand(UiCommand &command)
    {
        static char line[128];
        static uint8_t length = 0;
        static bool discardLine = false;

        while (LCD_SERIAL.available() > 0)
        {
            const int next = LCD_SERIAL.read();
            if (next < 0)
                break;

            const char c = (char)next;
            if (c == '\r')
                continue;

            if (c == '\n')
            {
                if (discardLine)
                {
#ifdef GEEETECH_DISPLAY_DEBUG
                    SERIAL_ECHOLNPGM("Dropped oversized display command");
#endif
                    discardLine = false;
                    length = 0;
                    continue;
                }

                line[length] = '\0';
                length = 0;

                String receivedLine(line);
                const int checksumStart = receivedLine.lastIndexOf('*');
                if (checksumStart > 0)
                    receivedLine.remove(checksumStart);

                // Some display commands contain the N-0 prefix twice.
                while (receivedLine.startsWith("N-0 "))
                    receivedLine = receivedLine.substring(4);

                command = parseCommandString(receivedLine);
                return true;
            }

            if (discardLine)
                continue;

            if (length >= sizeof(line) - 1)
            {
                discardLine = true;
                length = 0;
            }
            else
                line[length++] = c;
        }

        return false;
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT