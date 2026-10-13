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
 * lcd/extui/geeetech_a30t/geeetech_a30t_M2120.cpp
 *
 * This file implements the M2120 command controling the autoleveling
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
    void TouchDisplay::handle_M2120_AutoLeveling(const UiCommand &command)
    {
        bool sendProbeOffset = false;
        bool sendPosition = false;
        bool sendLevelingStatus = false;

        switch (command.parameters[P].charAt(0))
        {
        case '0': // auto leveling on/off
            autoLevelingEnabled = '1' == command.parameters[S].charAt(0);
            setLevelingActive(autoLevelingEnabled);
            if ('1' == command.parameters[S].charAt(0) && !getLevelingActive())
                SERIAL_ERROR_MSG("Bed leveling could not be enabled: invalid mesh.");
            sendProbeOffset = true;
            sendPosition = true;
            sendLevelingStatus = true;
            break;
        case '1': // control probe
            handle_M2120_P1_ProbeControl(command.parameters[S].charAt(0));
            break;
        case '2': // store Z offset value
            handle_M2120_P2_StoreZOffset(command.parameters[S]);
            sendProbeOffset = true;
            break;
        case '3': // offset up
            handle_M2120_P3_MoveUp(command.parameters[S].charAt(0));
            sendPosition = true;
            break;
        case '4': // offset down
            handle_M2120_P4_MoveDown(command.parameters[S].charAt(0));
            sendPosition = true;
            break;
        case '5': // start and request Z offset value
            disableAxisStatusSend = true;
            probeOffsetDisplayActive = true;
            probeOffsetCalibrationActive = false;
            sendProbeOffset = true;
            sendPosition = true;
            sendLevelingStatus = true;
            break;
        case '6': // move nozzle to center
            handle_M2120_P6_CenterNozzle();
            sendPosition = true;
            break;
        case '7': // move probe to center and measure
            disableAxisStatusSend = true;
            probeOffsetDisplayActive = true;
            handle_M2120_P7_ProbeCenter();
            sendPosition = true;
            sendLevelingStatus = true;
            break;
        case '9': // select CAS/BLtouch
        default:
#ifdef GEEETECH_DISPLAY_DEBUG
            SERIAL_ECHOLNPGM("Did not handle M2120 with P", command.parameters[P].charAt(0));
#endif
            break;
        }

        if (sendProbeOffset)
            send_L11_ProbeZOffset();
        if (sendPosition)
            send_L1_AxisInfo(true, true);
        if (sendLevelingStatus)
            send_L3_PrintInfo(true);
    }

    void TouchDisplay::handle_M2120_P1_ProbeControl(const char &sParameter)
    {
        // BLtouch control
        switch (sParameter)
        {
        case '0':         // pin up
            probe.stow(); // stow probe
            break;
        case '1':           // pin down
            probe.deploy(); // deploy probe
            break;
        default:                         // alarm release (case '2')
            handleGcode("M280 P0 S160"); // control servo
            break;
        }
    }

    void TouchDisplay::handle_M2120_P2_StoreZOffset(const String &sParameter)
    {
        const float_t offset = strtof(sParameter.c_str(), nullptr);
        if (!WITHIN(offset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
            SERIAL_ERROR_MSG("Z probe offset out of range (", Z_PROBE_OFFSET_RANGE_MIN, " to ", Z_PROBE_OFFSET_RANGE_MAX, ").");
            return;
        }
        setProbeOffset_mm(offset, ExtUI::Z);
        probeOffsetCalibrationActive = false;
        settingsStatusPending = true;
        settings.save();
    }

    void TouchDisplay::handle_M2120_P3_MoveUp(const char &sParameter)
    {
        const float_t height = mapSParameterToHeight(sParameter);
        const float_t offset = (probeOffsetCalibrationActive ? probeOffsetCalibrationValue : getProbeOffset_mm(ExtUI::Z)) + height;
        if (!WITHIN(offset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
            SERIAL_ERROR_MSG("Z probe offset out of range (", Z_PROBE_OFFSET_RANGE_MIN, " to ", Z_PROBE_OFFSET_RANGE_MAX, ").");
            return;
        }
        if (probeOffsetCalibrationActive)
            probeOffsetCalibrationValue = offset;
        else
            setProbeOffset_mm(offset, ExtUI::Z);
        do_blocking_move_to_z(current_position.z + height);
        settingsStatusPending = true;
    }

    void TouchDisplay::handle_M2120_P4_MoveDown(const char &sParameter)
    {
        const float_t height = mapSParameterToHeight(sParameter);
        const float_t offset = (probeOffsetCalibrationActive ? probeOffsetCalibrationValue : getProbeOffset_mm(ExtUI::Z)) - height;
        if (!WITHIN(offset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
            SERIAL_ERROR_MSG("Z probe offset out of range (", Z_PROBE_OFFSET_RANGE_MIN, " to ", Z_PROBE_OFFSET_RANGE_MAX, ").");
            return;
        }
        if (probeOffsetCalibrationActive)
            probeOffsetCalibrationValue = offset;
        else
            setProbeOffset_mm(offset, ExtUI::Z);
        do_blocking_move_to_z(current_position.z - height);
        settingsStatusPending = true;
    }

    void TouchDisplay::handle_M2120_P6_CenterNozzle()
    {
        // NO-OP, moving already done in "probe center"
    }

    void TouchDisplay::handle_M2120_P7_ProbeCenter()
    {
        setLevelingActive(false);
        gcode.home_all_axes(false);
        setLevelingActive(false);
        probeOffsetCalibrationActive = false;
        const float_t originalProbeOffset = getProbeOffset_mm(ExtUI::Z);
        setProbeOffset_mm(0, ExtUI::Z);
        settingsStatusPending = true;
        const float_t measuredZ = probe.probe_at_point(X_CENTER, Y_CENTER, PROBE_PT_STOW);
        setProbeOffset_mm(originalProbeOffset, ExtUI::Z);
        if (isnan(measuredZ)) {
            SERIAL_ERROR_MSG("Probe measurement failed.");
            do_blocking_move_to_z(5);
            do_blocking_move_to_xy(X_CENTER, Y_CENTER);
            return;
        }

        probeOffsetCalibrationActive = true;
        probeOffsetCalibrationValue = 0;
        // probe_at_point targets the bed with the probe, leaving the nozzle
        // offset from that point. Move the nozzle over the probed point before
        // starting the display's manual Z adjustment.
        current_position += probe.offset_xy;
        do_blocking_move_to_xy(current_position);
        do_blocking_move_to_z(measuredZ);
    }

    float_t TouchDisplay::mapSParameterToHeight(const char &sParameter)
    {
        switch (sParameter)
        {
        case '0':
            return 0.01; // The display's 10mm selection is used for fine Z adjustment.
        case '1':
            return 1;
        case '2':
            return 0.1;
        case '3':
        default:
            return 0.05;
        }
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT