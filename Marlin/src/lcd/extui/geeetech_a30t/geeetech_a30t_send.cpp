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

#ifndef MAX
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
    bool TouchDisplay::probeOffsetStatusPending = false;
    char TouchDisplay::output[] = {};

    void TouchDisplay::setNextSendMs(const millis_t &currentTimeMs) { nextStatusSend = currentTimeMs; }

    void TouchDisplay::sendStatusIfNeeded(const millis_t &currentTimeMs)
    {
        if (ELAPSED(currentTimeMs, nextStatusSend))
        {
            if (probeOffsetStatusPending) {
                send_L11_ProbeZOffset();
                probeOffsetStatusPending = false;
            }
            if (!disableAxisStatusSend || probeOffsetDisplayActive)
                send_L1_AxisInfo(false, probeOffsetDisplayActive);
            if (!disableAxisStatusSend) {
                if (settingsStatusPending) {
                    send_L24_SettingsStatus();
                    settingsStatusPending = false;
                }
            }
            send_L2_TempInfo();
            send_L3_PrintInfo();
            if (mixerStatusPending) {
                send_L22_MixingStatus();
                mixerStatusPending = false;
            }
            nextStatusSend = currentTimeMs + SEND_CYCLE_IN_MS;
        }
    }

    void TouchDisplay::send_L1_AxisInfo(const bool force, const bool useProbeOffset)
    {
        static char lastMessage[sizeof(output)];
        static bool messageSent = false;
        char x[16], y[16], z[16];

        xyze_pos_t currentLogicalPos = current_position.asLogical();
        dtostrf(currentLogicalPos.x, 0, 3, x);
        dtostrf(currentLogicalPos.y, 0, 3, y);
        const float_t displayedProbeOffset = probeOffsetCalibrationActive
            ? probeOffsetCalibrationValue
            : getProbeOffset_mm(ExtUI::Z);
        dtostrf(useProbeOffset ? displayedProbeOffset : currentLogicalPos.z, 0, 3, z);

        snprintf(output, sizeof(output), "L1 X%s Y%s Z%s F%d",
                 x /*7*/, y /*7*/, z /*7*/, FEEDRATE /*3*/);
        if (force || !messageSent || strcmp(output, lastMessage)) {
            sendToDisplay(output);
            strcpy(lastMessage, output);
            messageSent = true;
        }
    }

    void TouchDisplay::send_L2_TempInfo()
    {
        static char lastMessage[sizeof(output)];
        static bool messageSent = false;
        char bedCurrentTemp[6], bedTargetTemp[6], e0CurrentTemp[6], e0TargetTemp[6];

        syncDisplayTempTargets();
        dtostrf(getActualTemp_celsius(BED), 0, 1, bedCurrentTemp);
        dtostrf(displayBedTarget, 0, 1, bedTargetTemp);
        dtostrf(getActualTemp_celsius(E0), 0, 1, e0CurrentTemp);
        dtostrf(displayHotendTarget, 0, 1, e0TargetTemp);

        snprintf(output, sizeof(output), "L2 B:%s /%s /%d T0:%s /%s /%d T1:%s /%s /%d T2:%s /%s /%d SD:%d F0:%d F2:50 R:%d FR:%d",
                 bedCurrentTemp /*5*/, bedTargetTemp /*5*/, displayBedTarget > 0 /*1*/,
                 e0CurrentTemp /*5*/, e0TargetTemp /*5*/, displayHotendTarget > 0 /*1*/,
                 e0CurrentTemp /*5*/, e0TargetTemp /*5*/, displayHotendTarget > 0 /*1*/,
                 e0CurrentTemp /*5*/, e0TargetTemp /*5*/, displayHotendTarget > 0 /*1*/,
                 SD_STATUS /*1*/, F0_SPEED /*3*/, PRINT_SPEED /*3*/, FEEDRATE /*3*/);

        if (!messageSent || strcmp(output, lastMessage)) {
            sendToDisplay(output);
            strcpy(lastMessage, output);
            messageSent = true;
        }
    }

    void TouchDisplay::send_L3_PrintInfo(const bool force)
    {
        static char lastMessage[sizeof(output)];
        static bool messageSent = false;
        snprintf(output, sizeof(output), "L3 PS:%d VL:0 MT:%d FT:%d AL:%d ST:1 WF:0 MR:%lu FN:%.64s PG:%d TM:%lu LA:0 LC:0",
                 getPrintStatus() /*1*/, MOTOR_TENSION_STATUS /*1*/, getFilamentSensorStatus() /*3*/,
                 autoLevelingEnabled /*1*/, (unsigned long)getMixerRatio() /*7*/, CURRENT_FILENAME /*12*/,
                 getProgress_percent() /*3*/, (unsigned long)getProgress_seconds_elapsed() /*6*/);

        if (force || !messageSent || strcmp(output, lastMessage)) {
            sendToDisplay(output);
            strcpy(lastMessage, output);
            messageSent = true;
        }
    }

    void TouchDisplay::send_L7_FileList()
    {
        sendToDisplay("L7 begin file list:0");

        #if ENABLED(SDSUPPORT)
          if (isMediaInserted()) {
              FileList files;
              const uint16_t count = files.count();
              for (uint16_t i = 0; i < count; ++i) {
                  if (!files.seek(i)) {
                      #ifdef GEEETECH_DISPLAY_DEBUG
                        SERIAL_ECHOLNPGM("Could not read SD file list entry ", i);
                      #endif
                      break;
                  }
                  snprintf(output, sizeof(output), "L7 P%u %.64s", i, files.filename());
                  sendToDisplay(output);
              }
          }
        #endif

        sendToDisplay("L7 end file list:0");
    }

    void TouchDisplay::send_L22_MixingStatus()
    {
        #if ENABLED(GRADIENT_MIX)
          snprintf(output, sizeof(output), "L22 MS:%u MR:%lu SP:100 EP:100 SH:%.2f EH:%.2f",
                   mixingMode, (unsigned long)getMixerRatio(),
                   mixer.gradient.start_z, mixer.gradient.end_z);
        #else
          snprintf(output, sizeof(output), "L22 MS:%u MR:%lu SP:100 EP:100 SH:0.00 EH:0.00",
                   mixingMode, (unsigned long)getMixerRatio());
        #endif
        sendToDisplay(output);
    }

    void TouchDisplay::syncDisplayTempTargets()
    {
        const millis_t now = millis();
        if (!displayTempTargetsValid || ELAPSED(now, nextDisplayTempTargetSync)) {
            displayBedTarget = getTargetTemp_celsius(BED);
            displayHotendTarget = getTargetTemp_celsius(E0);
            displayTempTargetsValid = true;
            nextDisplayTempTargetSync = now + 5000;
        }
    }

    void TouchDisplay::send_L9_FirmwareInfo()
    {
        snprintf(output, sizeof(output), "L9 DN:Geeetech;DM:A30T;SN:TheThomasD;FV:Marlin " SHORT_BUILD_VERSION ";PV:320.00 x 320.00 x 420.00;HV:GTM32_103_V1;");

        sendToDisplay(output);
    }

    void TouchDisplay::send_L10_ZOffset()
    {
        char zOffset[7];
        dtostrf(-manualLevelingOffset, 0, 2, zOffset);
        snprintf(output, sizeof(output), "L10 S%s", zOffset);
        sendToDisplay(output);
    }

    void TouchDisplay::send_L11_ProbeZOffset()
    {
        char probeZOffset[7];
        dtostrf(getProbeOffset_mm(ExtUI::Z), 0, 2, probeZOffset);
        snprintf(output, sizeof(output), "L11 P0 S%s", probeZOffset);
        sendToDisplay(output);
    }

    void TouchDisplay::send_L14_MessageToDisplay(const String &message)
    {
        snprintf(output, sizeof(output), "L14 %s", message.c_str());
        sendToDisplay(output);
    }

    void TouchDisplay::send_L18_UserMessage(const UserMessageCode &code)
    {
        snprintf(output, sizeof(output), "L18 P26 S%d", (int)code);
        sendToDisplay(output);
    }

    void TouchDisplay::send_L24_SettingsStatus()
    {
        char varA[10];
        char varB[10];
        char varC[10];
        char varD[10];

        // steps/mm: A=X, B=Y, C=Z, D=E(one for all)
        dtostrf(planner.settings.axis_steps_per_mm[X_AXIS], 0, 2, varA);
        dtostrf(planner.settings.axis_steps_per_mm[Y_AXIS], 0, 2, varB);
        dtostrf(planner.settings.axis_steps_per_mm[Z_AXIS], 0, 2, varC);
        dtostrf(planner.settings.axis_steps_per_mm[E_AXIS], 0, 2, varD);
        snprintf(output, sizeof(output), "L24 P0 A%s B%s C%s D%s", varA, varB, varC, varD);
        sendToDisplay(output);

        // velocity (mm/s): A=X-VMax B=Y-VMax C=Z-VMax D=E-VMax E=VMin F=VTravel
        snprintf(output, sizeof(output), "L24 P1 A%d B%d C%d D%d E%d F%d",
                /*A*/ (int)planner.settings.max_feedrate_mm_s[X_AXIS],
                /*B*/ (int)planner.settings.max_feedrate_mm_s[Y_AXIS],
                /*C*/ (int)planner.settings.max_feedrate_mm_s[Z_AXIS],
                /*D*/ (int)planner.settings.max_feedrate_mm_s[E_AXIS],
                /*E*/ MAX((int)planner.settings.min_feedrate_mm_s, 1),
                /*F*/ MAX((int)planner.settings.min_travel_feedrate_mm_s, 1));
        sendToDisplay(output);

        // acceleration (steps/s2): A=Accel, B=A-Retract, C=X-Max accel, D=Y-Max accel, E=Z-Max accel, F=E-Max accel
        snprintf(output, sizeof(output), "L24 P2 A%d B%d C%d D%d E%d F%d",
                /*A*/ (int)planner.settings.acceleration,
                /*B*/ (int)fwretract.settings.retract_feedrate_mm_s,
                /*C*/ (int)planner.settings.max_acceleration_mm_per_s2[X_AXIS],
                /*D*/ (int)planner.settings.max_acceleration_mm_per_s2[Y_AXIS],
                /*E*/ (int)planner.settings.max_acceleration_mm_per_s2[Z_AXIS],
                /*F*/ (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
        sendToDisplay(output);

        //jerk (mm/s): A=Vx-jerk, B=Vy-jerk, C=Vz-jerk, D=Ve-jerk
        dtostrf(planner.max_jerk.x, 0, 2, varA);
        dtostrf(planner.max_jerk.y, 0, 2, varB);
        dtostrf(planner.max_jerk.z, 0, 2, varC);
        dtostrf(planner.max_jerk.e, 0, 2, varD);
        snprintf(output, sizeof(output), "L24 P3 A%s B%s C%s D%s", varA, varB, varC, varD);
        sendToDisplay(output);

        // babystep (mm): A=Z
        dtostrf(temporaryBabystepValue, 0, 2, varA);
        snprintf(output, sizeof(output), "L24 P5 A%s", varA);
        sendToDisplay(output);

        // double-z home offset
        dtostrf(endstops.z2_endstop_adj > 0 ? endstops.z2_endstop_adj : 0.0, 0, 2, varA);
        dtostrf(endstops.z2_endstop_adj < 0 ? -endstops.z2_endstop_adj : 0.0, 0, 2, varB);
        snprintf(output, sizeof(output), "L24 P6 A%s B%s", varA, varB);
        sendToDisplay(output);
    }

    void TouchDisplay::sendToDisplay(const char *const message, const bool addChecksum)
    {
        char line[sizeof(output) + 16];
        if (addChecksum)
        {
            uint8_t checksum = 115; // checksum of line sent above
            for (const char *p = message; *p; ++p)
                checksum ^= *p;
            snprintf(line, sizeof(line), "N-0 %s*%u", message, checksum);
        }
        else
            snprintf(line, sizeof(line), "%s", message);

        LCD_SERIAL.write(line);
        LCD_SERIAL.write("\r\n");
#ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOPGM("A30T LCD TX: ");
        SERIAL_ECHOLN(line);
#endif
        delay(20);
    }

} // namespace Geeetech

#endif // GEEETECH_A30T_TFT