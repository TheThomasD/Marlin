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
 * lcd/extui/geeetech_a30t/geeetech_a30t_base.cpp
 *
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
    const char *const COMMAND_STRINGS[] = {FOREACH_COMMAND(GENERATE_STRING)};
    const char *const PARAMETER_STRINGS[] = {FOREACH_PARAMETER(GENERATE_STRING)};

    bool TouchDisplay::shouldWaitForCommand = false;
    bool TouchDisplay::simulatedAutoLevelSwitchOn = true;
    CommandType TouchDisplay::activeCommand = Unknown;

    void TouchDisplay::startup()
    {
        // use pin PD12 (60) as output and pull it up to supply 5V power
        SET_OUTPUT(GEEETECH_A30T_TFT_POWER_PIN);
        WRITE(GEEETECH_A30T_TFT_POWER_PIN, 1);

        // setup the communication with the display
        LCD_SERIAL.setRx(LCD_UART_RX); // use PD9 for RX
        LCD_SERIAL.setTx(LCD_UART_TX); // use PD8 for TX
        LCD_SERIAL.begin(GEEETECH_A30T_TFT_BAUDRATE);

#ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Geeetech A30T TFT initialized");
#endif
    }

    void TouchDisplay::waitForCommand(const bool wait) { shouldWaitForCommand = wait; }

    void TouchDisplay::process()
    {
        const millis_t currentTimeMs = millis();
        if (!shouldWaitForCommand)
        {
            UiCommand command = {};
            if (receiveCommand(command))
            {

                // Ignore unknown command types for keeping command group active.
                if (Unknown != command.type)
                {
                    // A new command group re-enables axis status updates.
                    if (disableAxisStatusSend && activeCommand != command.type)
                        disableAxisStatusSend = false;
                    activeCommand = command.type;
                }

#ifdef GEEETECH_DISPLAY_DEBUG
                SERIAL_ECHOLNPGM("CommandType: ", COMMAND_STRINGS[command.type]);
                SERIAL_ECHOLNPGM("String: ", command.command.c_str());
                for (uint8_t j = 0; j < PARAMETERS_COUNT; j++)
                    SERIAL_ECHOLNPGM(PARAMETER_STRINGS[j], command.parameters[j].c_str());
#endif

                if (GCode == command.type)
                    handleGcode(command.command);
                else if (Unknown == command.type)
                    handleUnkownCommand(command);
                else
                    handleProprietaryCommand(command);

                // Schedule an immediate answer, if needed.
                if (false FOREACH_ANSWER(command.type, GENERATE_BOOL_COMPARE))
                    setNextSendMs(currentTimeMs);
            }
        }

        extrudeIfNeeded(currentTimeMs);
        sendStatusIfNeeded(currentTimeMs);
    }

    void TouchDisplay::showStatus(const char *const message) {
        send_L14_MessageToDisplay(message);
    }

    void TouchDisplay::showUserMessage(const UserMessageCode &code) {
        send_L18_UserMessage(code);
    }

    void TouchDisplay::requestStatusUpdate() {
        setNextSendMs(millis());
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT