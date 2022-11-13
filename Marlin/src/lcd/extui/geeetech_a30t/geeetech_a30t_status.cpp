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
 * lcd/extui/geeetech_a30t/geeetech_a30t_status.cpp
 *
 * This handles providing and collecting status information
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
    uint32_t TouchDisplay::getMixerRatio()
    {
        mixer_comp_t block[MIXING_STEPPERS];
        mixer.populate_block(block);

        const float_t sum = block[0] + block[1] + block[2];

        uint8_t percentageE0 = rintf(100.0 * block[0] / sum);
        uint8_t percentageE1 = rintf(100.0 * block[1] / sum);
        uint8_t percentageE2 = rintf(100.0 * block[2] / sum);

        const uint8_t diff = 100 - percentageE0 - percentageE1 - percentageE2;

        if (diff != 0)
        {
            if (percentageE0 >= percentageE1 && percentageE0 >= percentageE2)
                percentageE0 += diff;
            else if (percentageE1 >= percentageE2)
                percentageE1 += diff;
            else
                percentageE2 += diff;
        }

        uint32_t result = percentageE2;
        result = (result << 8) + percentageE1;
        result = (result << 8) + percentageE0;

        return result;
    }

    uint8_t TouchDisplay::getPrintStatus()
    {
        if (printJobOngoing() || isPrintingFromMedia())
            return PRINT_STATUS_PRINTING;
        else if (isPrintingPaused() || isPrintingFromMediaPaused())
            return PRINT_STATUS_PAUSED;
        else if (card.isFileOpen() && card.eof())
            return PRINT_STATUS_FINISHED;
        return PRINT_STATUS_IDLE;
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT