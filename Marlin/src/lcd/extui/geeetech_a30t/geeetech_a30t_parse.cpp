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
 * lcd/extui/geeetech_a30t/geeetech_a30t_parse.cpp
 *
 * This file handles parsing of commands coming from the display
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
    UiCommand TouchDisplay::parseCommandString(const String &commandString)
    {
        UiCommand result = {}; // init
        result.type = parseCommandType(commandString);

        if (GCode == result.type)
            result.command = commandString;
        else if (Unknown != result.type)
            parseCommandParameters(result, commandString);

        return result;
    }

    void TouchDisplay::parseCommandParameters(UiCommand &command, const String &commandString)
    {
        String currentCommand = COMMAND_STRINGS[command.type];
        if (commandString.length() + 1 <= currentCommand.length())
            return; // no parameters detected

        String parameterString = commandString.substring(currentCommand.length() + 1);
        for (uint8_t i = 0; i < PARAMETERS_COUNT; i++)
        {
            parameterString.trim();
            if (parameterString.length() == 0)
                break;

            if (parameterString.startsWith(PARAMETER_STRINGS[i]))
            {
                uint8_t substringStart = i == FW ? 3 : strlen(PARAMETER_STRINGS[i]); // need to handle FW differently as it is followed by a colon
                int index = parameterString.indexOf(' ');
                if (index > substringStart)
                {
                    command.parameters[i] = parameterString.substring(substringStart, index);
                    parameterString = parameterString.substring(index);
                }
                else
                {
                    command.parameters[i] = parameterString.substring(substringStart);
                    break;
                }
            }
        }
    }

    CommandType TouchDisplay::parseCommandType(const String &commandString)
    {
        const char firstChar = commandString.charAt(0);

        if ('M' == firstChar || 'L' == firstChar)        // check proprietary commands first
            for (uint8_t i = 2; i < COMMANDS_COUNT; i++) // start at 2 to exclude Unknown and GCode
            {
                if (commandString.startsWith(COMMAND_STRINGS[i]))
                    return (CommandType)i;
            }

        if ('M' == firstChar || ('G' == firstChar && 'e' != commandString.charAt(1))) // filter out Geeetech
            return GCode;

        return Unknown;
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT