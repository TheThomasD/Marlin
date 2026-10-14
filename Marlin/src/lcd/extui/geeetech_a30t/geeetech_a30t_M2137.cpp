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

#if ENABLED(GEEETECH_A30T_TFT) && ENABLED(GRADIENT_MIX)

#include "geeetech_a30t.h"
#include "../ui_api.h"

using namespace ExtUI;

namespace Geeetech {

  static bool parseMixRatio(const String &parameter, float (&mix)[MIXING_STEPPERS]) {
    if (!parameter.length()) return false;

    char *end;
    const unsigned long parsed = strtoul(parameter.c_str(), &end, 10);
    if (end == parameter.c_str() || *end || parsed > 0xFFFFFFUL) return false;

    const uint32_t packed = uint32_t(parsed);
    uint32_t sum = 0;
    MIXER_STEPPER_LOOP(i) {
      const uint8_t value = uint8_t(packed >> (8 * i));
      if (value > 100) return false;
      mix[i] = value;
      sum += value;
    }
    return sum > 0;
  }

  static bool parseHeight(const String &parameter, float &height) {
    if (!parameter.length()) return false;
    char *end;
    height = strtof(parameter.c_str(), &end);
    return end != parameter.c_str() && !*end && isfinite(height) && WITHIN(height, Z_MIN_POS, Z_MAX_POS);
  }

  static void setTemplateMix(const float (&mix)[MIXING_STEPPERS], const uint8_t vtool) {
    float savedCollector[MIXING_STEPPERS];
    MIXER_STEPPER_LOOP(i) {
      savedCollector[i] = mixer.collector[i];
      mixer.set_collector(i, mix[i]);
    }
    mixer.normalize(vtool);
    MIXER_STEPPER_LOOP(i) mixer.collector[i] = savedCollector[i];
  }

  void TouchDisplay::handle_M2137_SetMixingTemplate(const UiCommand &command) {
    const String &startRatio = command.parameters[A];
    const String &endRatio = command.parameters[B];
    const String &startHeight = command.parameters[C];
    const String &endHeight = command.parameters[D];
    float startMix[MIXING_STEPPERS], endMix[MIXING_STEPPERS];
    float startZ, endZ;

    if (startRatio.length() && !parseMixRatio(startRatio, startMix)) {
      #ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Invalid M2137 start mix ratio");
      #endif
      return;
    }
    if (endRatio.length() && !parseMixRatio(endRatio, endMix)) {
      #ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Invalid M2137 end mix ratio");
      #endif
      return;
    }
    if (startHeight.length() && !parseHeight(startHeight, startZ)) {
      #ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Invalid M2137 start height");
      #endif
      return;
    }
    if (endHeight.length() && !parseHeight(endHeight, endZ)) {
      #ifdef GEEETECH_DISPLAY_DEBUG
        SERIAL_ECHOLNPGM("Invalid M2137 end height");
      #endif
      return;
    }

    if (startRatio.length()) {
      setTemplateMix(startMix, TEMPLATE_START_VTOOL);
      mixer.gradient.start_vtool = TEMPLATE_START_VTOOL;
    }

    if (endRatio.length()) {
      setTemplateMix(endMix, TEMPLATE_END_VTOOL);
      mixer.gradient.end_vtool = TEMPLATE_END_VTOOL;
    }

    if (startHeight.length()) mixer.gradient.start_z = startZ;
    if (endHeight.length()) mixer.gradient.end_z = endZ;

    if (mixingMode == (uint16_t(3) << 8))
      mixer.refresh_gradient();
    else
      mixer.gradient.enabled = false;
    mixerStatusPending = true;
  }

}

#endif // GEEETECH_A30T_TFT && GRADIENT_MIX
