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
 * lcd/extui/geeetech_a30t/geeetech_a30t_extui.cpp
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(GEEETECH_A30T_TFT)

#include "../ui_api.h"
#include "geeetech_a30t.h"

using namespace Geeetech;

namespace ExtUI
{
  void onStartup() { Display.startup(); }
  void onIdle() { Display.process(); }

  void onFilamentRunout(const extruder_t extruder) { Display.showUserMessage(NoFilament); }

  void onPrinterKilled(FSTR_P const error, FSTR_P const) { Display.showStatus(FTOP(error)); }

  void onPrintTimerStarted() { Display.requestStatusUpdate(); }
  void onPrintTimerPaused() { Display.requestStatusUpdate(); }
  void onPrintTimerStopped() { Display.requestStatusUpdate(); }
  void onPrintDone() { Display.requestStatusUpdate(); }

  void onStatusChanged(const char *const msg) { Display.showStatus(msg); }

  void onMediaInserted() { Display.requestStatusUpdate(); }
  void onMediaError() { Display.showStatus("Media error"); }
  void onMediaRemoved() {
    Display.showUserMessage(SdRemoved);
    Display.requestStatusUpdate();
  }

  void onPlayTone(const uint16_t, const uint16_t) {}

  void onUserConfirmRequired(const char *const msg) {
    Display.showUserMessage(AskWaitForUser);
    Display.showStatus(msg);
  }

  void onHomingStart() { Display.waitForCommand(true); }
  void onHomingDone() {
    Display.waitForCommand(false);
    Display.requestStatusUpdate();
  }

  void onFactoryReset() { Display.requestStatusUpdate(); }
  void onStoreSettings(char *) {}
  void onSettingsStored(bool) { Display.requestStatusUpdate(); }
  void onLoadSettings(const char *) {}
  void onSettingsLoaded(bool) { Display.requestStatusUpdate(); }
  void onPostprocessSettings() {}
  void onConfigurationStoreWritten(bool) {}
  void onConfigurationStoreRead(bool) {}

  #if HAS_MESH
    void onLevelingStart() { Display.waitForCommand(true); }
    void onLevelingDone() {
      Display.waitForCommand(false);
      Display.requestStatusUpdate();
    }
    void onMeshUpdate(const int8_t, const int8_t, const_float_t) {}
    void onMeshUpdate(const int8_t, const int8_t, const ExtUI::probe_state_t state) {
      if (state == ExtUI::G29_START)
        Display.waitForCommand(true);
      else if (state == ExtUI::G29_FINISH) {
        Display.waitForCommand(false);
        Display.requestStatusUpdate();
      }
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {}
  #endif

  void onSteppersDisabled() {}
  void onSteppersEnabled() {}

#if HAS_PID_HEATING
  void onPidTuning(const result_t rst)
  {
  }
#endif
}

#endif // GEEETECH_A30T_TFT
