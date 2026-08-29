#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "displays/touch/touch_config.hpp"

#ifndef TOUCH_I2C_ADDR
#define TOUCH_I2C_ADDR 0x38
#endif

class LovyanFT5x06Touch {
  lgfx::Touch_FT5x06 _touch_instance;

public:
  template <typename Panel> void attach(Panel &panel) {
    auto cfg = _touch_instance.config();

    cfg.x_min = 0;
    cfg.x_max = SCREEN_WIDTH - 1;
    cfg.y_min = 0;
    cfg.y_max = SCREEN_HEIGHT - 1;
    cfg.pin_int = TOUCH_IRQ;
    cfg.bus_shared = true;
    cfg.offset_rotation = 0;
    cfg.i2c_port = 0;
    cfg.i2c_addr = TOUCH_I2C_ADDR;
    cfg.pin_sda = TOUCH_SDA;
    cfg.pin_scl = TOUCH_SCL;
    cfg.freq = 400000;

    _touch_instance.config(cfg);
    attachTouchDevice(panel, 0);
  }

  bool init(void) { return _touch_instance.init(); }
  void setRotation(uint8_t rotation) {}

  template <typename Panel> bool read(Panel &panel, uint16_t *x, uint16_t *y) {
    (void)panel;
    return readTouch(x, y);
  }

private:
  bool readTouch(uint16_t *x, uint16_t *y) {
    lgfx::touch_point_t point;
    if (!_touch_instance.getTouchRaw(&point, 1)) {
      return false;
    }

    auto cfg = _touch_instance.config();
    *x = mapAxis(point.x, cfg.x_min, cfg.x_max, SCREEN_WIDTH - 1);
    *y = mapAxis(point.y, cfg.y_min, cfg.y_max, SCREEN_HEIGHT - 1);
    return true;
  }

  static uint16_t mapAxis(int32_t value, int32_t in_min, int32_t in_max,
                          int32_t out_max) {
    if (in_min == in_max) {
      return 0;
    }
    int32_t mapped = (value - in_min) * out_max / (in_max - in_min);
    return (uint16_t)constrain(mapped, 0, out_max);
  }

  template <typename Panel>
  auto attachTouchDevice(Panel &panel, int)
      -> decltype(panel.setTouchDevice(&_touch_instance), void()) {
    panel.setTouchDevice(&_touch_instance);
  }

  template <typename Panel> void attachTouchDevice(Panel &, ...) {}
};
