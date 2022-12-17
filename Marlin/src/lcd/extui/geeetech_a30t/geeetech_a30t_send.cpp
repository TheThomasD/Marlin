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
 * lcd/extui/geeetech_a30t/geeetech_a30t_send.cpp
 *
 * This handles sending status to the display
 * 
 * written in 2021 by TheThomasD
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(GEEETECH_A30T_TFT)

#ifndef MAX(a, b)
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

#include "geeetech_a30t.h"
#include "../ui_api.h"
#include "../../marlinui.h"

using namespace ExtUI;

namespace Geeetech
{
    millis_t TouchDisplay::nextStatusSend = 0;
    bool TouchDisplay::disableAxisStatusSend = false;
    char TouchDisplay::output[] = {};

    void TouchDisplay::setNextSendMs(const millis_t &currentTimeMs) { nextStatusSend = currentTimeMs; }

    void TouchDisplay::sendStatusIfNeeded(const millis_t &currentTimeMs)
    {
        if (ELAPSED(currentTimeMs, nextStatusSend))
        {
            if (!disableAxisStatusSend)
            {
                send_L1_AxisInfo();
                send_L24_SettingsStatus();
            }
            send_L2_TempInfo();
            send_L3_PrintInfo();
            nextStatusSend = currentTimeMs + SEND_CYCLE_IN_MS;
        }
    }

    void TouchDisplay::send_L1_AxisInfo()
    {
        char x[8], y[8], z[8]; // 6 digits + dot + \0

        xyze_pos_t currentLogicalPos = current_position.asLogical();
        dtostrf(currentLogicalPos.x, 0, 3, x);
        dtostrf(currentLogicalPos.y, 0, 3, y);
        dtostrf(currentLogicalPos.z, 0, 3, z);

        sprintf(output, "L1 X%s Y%s Z%s F%d",
                x /*7*/, y /*7*/, z /*7*/, FEEDRATE /*3*/);
        sendToDisplay(output);
    }

    void TouchDisplay::send_L2_TempInfo()
    {
        char bedCurrentTemp[6], bedTargetTemp[6], e0CurrentTemp[6], e0TargetTemp[6];

        dtostrf(getActualTemp_celsius(BED), 0, 1, bedCurrentTemp);
        dtostrf(getTargetTemp_celsius(BED), 0, 1, bedTargetTemp);
        dtostrf(getActualTemp_celsius(E0), 0, 1, e0CurrentTemp);
        dtostrf(getTargetTemp_celsius(E0), 0, 1, e0TargetTemp);

        sprintf(output, "L2 B:%s /%s /%d T0:%s /%s /%d T1:%s /%s /%d T2:%s /%s /%d SD:%d F0:%d F2:50 R:%d FR:%d",
                bedCurrentTemp /*5*/, bedTargetTemp /*5*/, BED_ACTIVE /*1*/,
                e0CurrentTemp /*5*/, e0TargetTemp /*5*/, E0_ACTIVE /*1*/,
                e0CurrentTemp /*5*/, e0TargetTemp /*5*/, E0_ACTIVE /*1*/,
                e0CurrentTemp /*5*/, e0TargetTemp /*5*/, E0_ACTIVE /*1*/,
                SD_ACTIVE /*1*/, F0_SPEED /*3*/, PRINT_SPEED /*3*/, FEEDRATE /*3*/);

        sendToDisplay(output);
    }

    void TouchDisplay::send_L3_PrintInfo()
    {
        sprintf(output, "L3 PS:%d VL:0 MT:%d FT:%d AL:%d ST:1 WF:0 MR:%ld FN:%s PG:%d TM:%ld LA:0 LC:0",
                getPrintStatus() /*1*/, MOTOR_TENSION_STATUS /*1*/, FILAMENT_SENSOR_STATUS /*3*/,
                simulatedAutoLevelSwitchOn /*1*/, getMixerRatio() /*7*/, CURRENT_FILENAME /*12*/,
                getProgress_percent() /*3*/, getProgress_seconds_elapsed() /*6*/);

        sendToDisplay(output);
    }

    void TouchDisplay::send_L9_FirmwareInfo()
    {
        sprintf(output, "L9 DN:Geeetech;DM:A30T;SN:TheThomasD;FV:%s;PV:320.00 x 320.00 x 420.00;HV:GTM32_103_V1;", getFirmwareName_str());

        sendToDisplay(output);
    }

    void TouchDisplay::send_L10_ZOffset()
    {
        char zOffset[7];
        dtostrf(-home_offset[Z_AXIS], 0, 2, zOffset);
        sprintf(output, "L10 S%s", zOffset);
        sendToDisplay(output);
    }

    void TouchDisplay::send_L11_ProbeZOffset_andFakeL1()
    {
        // send L11 and fake L1 as it is required as well :(
        char probeZOffset[7];
        dtostrf(getProbeOffset_mm(ExtUI::Z), 0, 2, probeZOffset);
        sprintf(output, "%s", probeZOffset);
        sendToDisplay(("L1 Z" + (String)output).c_str());
        sendToDisplay(("L11 P0 S" + (String)output).c_str());
    }

    void TouchDisplay::send_L14_MessageToDisplay(const String &message)
    {
        sendToDisplay(("L14 " + message).c_str());
    }

    void TouchDisplay::send_L18_UserMessage(const UserMessageCode &code)
    {
        sendToDisplay(("L18 P26 S" + (String)code).c_str());
    }

    void TouchDisplay::send_L24_SettingsStatus()
    {
        const String start = "L24 P";
        char varA[10];
        char varB[10];
        char varC[10];
        char varD[10];

        // steps/mm: A=X, B=Y, C=Z, D=E(one for all)
        const String p0 = start + "0 A%s B%s C%s D%s";
        dtostrf(planner.settings.axis_steps_per_mm[X_AXIS], 0, 2, varA);
        dtostrf(planner.settings.axis_steps_per_mm[Y_AXIS], 0, 2, varB);
        dtostrf(planner.settings.axis_steps_per_mm[Z_AXIS], 0, 2, varC);
        dtostrf(planner.settings.axis_steps_per_mm[E_AXIS], 0, 2, varD);
        sprintf(output, p0.c_str(), varA, varB, varC, varD);
        sendToDisplay(output);

        // velocity (mm/s): A=X-VMax B=Y-VMax C=Z-VMax D=E-VMax E=VMin F=VTravel
        const String p1 = start + "1 A%d B%d C%d D%d E%d F%d";
        sprintf(output, p1.c_str(),
                /*A*/ (int)planner.settings.max_feedrate_mm_s[X_AXIS],
                /*B*/ (int)planner.settings.max_feedrate_mm_s[Y_AXIS],
                /*C*/ (int)planner.settings.max_feedrate_mm_s[Z_AXIS],
                /*D*/ (int)planner.settings.max_feedrate_mm_s[E_AXIS],
                /*E*/ MAX((int)planner.settings.min_feedrate_mm_s, 1),
                /*F*/ MAX((int)planner.settings.min_travel_feedrate_mm_s, 1));
        sendToDisplay(output);

        // acceleration (steps/s2): A=Accel, B=A-Retract, C=X-Max accel, D=Y-Max accel, E=Z-Max accel, F=E-Max accel
        const String p2 = start + "2 A%d B%d C%d D%d E%d F%d";
        sprintf(output, p2.c_str(),
                /*A*/ (int)planner.settings.acceleration,
                /*B*/ (int)fwretract.settings.retract_feedrate_mm_s,
                /*C*/ (int)planner.settings.max_acceleration_mm_per_s2[X_AXIS],
                /*D*/ (int)planner.settings.max_acceleration_mm_per_s2[Y_AXIS],
                /*E*/ (int)planner.settings.max_acceleration_mm_per_s2[Z_AXIS],
                /*F*/ (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
        sendToDisplay(output);

        //jerk (mm/s): A=Vx-jerk, B=Vy-jerk, C=Vz-jerk, D=Ve-jerk
        const String p3 = start + "3 A%s B%s C%s D%s";
        dtostrf(planner.max_jerk.x, 0, 2, varA);
        dtostrf(planner.max_jerk.y, 0, 2, varB);
        dtostrf(planner.max_jerk.z, 0, 2, varC);
        dtostrf(planner.max_jerk.e, 0, 2, varD);
        sprintf(output, p3.c_str(), varA, varB, varC, varD);
        sendToDisplay(output);

        // babystep (mm): A=Z
        const String p5 = start + "5 A%s";
        dtostrf(temporaryBabystepValue, 0, 2, varA);
        sprintf(output, p5.c_str(), varA);
        sendToDisplay(output);

        // double-z home offset
        const String p6 = start + "6 A%s B%s";
        dtostrf(endstops.z2_endstop_adj > 0 ? endstops.z2_endstop_adj : 0.0, 0, 2, varA);
        dtostrf(endstops.z2_endstop_adj < 0 ? -endstops.z2_endstop_adj : 0.0, 0, 2, varB);
        sprintf(output, p6.c_str(), varA, varB);
        sendToDisplay(output);
    }

    void TouchDisplay::sendToDisplay(PGM_P message, const bool addChecksum)
    {
        if (addChecksum)
        {
            LCD_SERIAL.write("N-0 ");

            uint8_t checksum = 115; // checksum of line sent above
            for (unsigned int i = 0; i < strlen(message); i++)
            {
                checksum ^= message[i];
                LCD_SERIAL.write(message[i]);
            }
            char number[5];
            sprintf(number, "*%d", checksum);
            LCD_SERIAL.write(number);
        }
        else
        {
            LCD_SERIAL.write(message);
        }

        LCD_SERIAL.write("\r\n");
        delay(20);
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT