#pragma once

#include <Arduino.h>

#include "TAMC_GT911.h"
#include "displays/touch/touch_config.hpp"

#ifndef TAMC_GT911_ROTATION
#define TAMC_GT911_ROTATION ROTATION_NORMAL
#endif

class GT911Touch {
public:
  TAMC_GT911 tp =
      TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_IRQ, TOUCH_RST, TOUCH_WIDTH,
                 TOUCH_HEIGHT);

  template <typename Panel> void attach(Panel &panel) {}
  bool init(void) {
    tp.begin();
    tp.setRotation(TAMC_GT911_ROTATION);
    return true;
  }
  void setRotation(uint8_t rotation) { tp.setRotation(rotation); }
  template <typename Panel> bool read(Panel &panel, uint16_t *x, uint16_t *y) {
    tp.read();
    *x = tp.points[0].x;
    *y = tp.points[0].y;
    return tp.isTouched;
  }
};
