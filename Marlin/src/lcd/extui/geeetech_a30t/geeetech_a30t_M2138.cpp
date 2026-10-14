/**
 * Marlin 3D Printer Firmware
 *
 * This file is part of Marlin.
 *
 * Marlin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(GEEETECH_A30T_TFT)

#include "geeetech_a30t.h"
#include "../ui_api.h"

using namespace ExtUI;

namespace Geeetech {

  void TouchDisplay::handle_M2138_SetMixingMode(const UiCommand &command) {
    const String &modeParameter = command.parameters[S];
    const char mode = modeParameter.charAt(0);
    if (!modeParameter.length() || (mode != '0' && mode != '1' && mode != '3') || modeParameter.length() != 1) {
      #ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Unsupported M2138 mixing mode: ", mode);
      #endif
      return;
    }

    mixingMode = uint16_t(mode - '0') << 8;
    #if ENABLED(GRADIENT_MIX)
      if (mode == '3') {
        mixer.T(TEMPLATE_START_VTOOL);
        mixer.refresh_gradient();
      }
      else {
        mixer.T(FIXED_MIX_VTOOL);
        mixer.gradient.enabled = false;
      }
    #else
      if (mode == '3') {
        #ifdef GEEETECH_DISPLAY_DEBUG
          SERIAL_ECHOLNPGM("Gradient mixing is not enabled");
        #endif
        mixingMode = 0;
        return;
      }
      mixer.T(FIXED_MIX_VTOOL);
    #endif
    mixerStatusPending = true;
  }

}

#endif // GEEETECH_A30T_TFT
