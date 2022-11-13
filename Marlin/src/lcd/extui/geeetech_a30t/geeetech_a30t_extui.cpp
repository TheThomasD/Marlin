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

  void onPrinterKilled(FSTR_P const error, FSTR_P const component) {}

  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onPrintDone() {}

  void onStatusChanged(const char *const msg) { Display.showStatus(msg); }

  void onMediaInserted() {}
  void onMediaError() {}
  void onMediaRemoved() { Display.showUserMessage(SdRemoved); }

  void onPlayTone(const uint16_t, const uint16_t) {}

  void onUserConfirmRequired(const char *const msg){};

  void onHomingStart() { Display.waitForCommand(true); }
  void onHomingDone() { Display.waitForCommand(false); }

  void onFactoryReset() {}
  void onStoreSettings(char *) {}
  void onSettingsStored(bool) {}
  void onLoadSettings(const char *) {}
  void onSettingsLoaded(bool) {}
  void onPostprocessSettings() {}
  void onConfigurationStoreWritten(bool) {}
  void onConfigurationStoreRead(bool) {}

  void onLevelingStart() {}
  void onLevelingDone() {}
  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval) {}
  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const ExtUI::probe_state_t state)
  {
    Display.waitForCommand(ExtUI::probe_state_t::G29_FINISH == state);
  }

  void onPowerLossResume() {}

  void onSteppersDisabled() {}
  void onSteppersEnabled() {}

#if ENABLED(POWER_LOSS_RECOVERY)
  void onPowerLossResume()
  {
  }
#endif
#if HAS_PID_HEATING
  void onPidTuning(const result_t rst)
  {
  }
#endif
}

#endif // GEEETECH_A30T_TFT
