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
    const char mode = command.parameters[S].charAt(0);
    if (mode != '0' && mode != '1' && mode != '3') {
      #ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Unsupported M2138 mixing mode: ", mode);
      #endif
      return;
    }

    // The display protocol encodes its mode in the high byte of the L22 value.
    mixingMode = uint16_t(mode - '0') << 8;
    mixerStatusPending = true;
  }

}

#endif // GEEETECH_A30T_TFT
