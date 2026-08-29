#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "displays/touch/touch_config.hpp"

#ifndef TOUCH_RST
#define TOUCH_RST -1
#endif

class LovyanXPT2046Touch {
  lgfx::Touch_XPT2046 _touch_instance;

public:
  template <typename Panel> void attach(Panel &panel) {
    auto cfg = _touch_instance.config();

    cfg.x_min = 0;
    cfg.x_max = SCREEN_WIDTH - 1;
    cfg.y_min = 0;
    cfg.y_max = SCREEN_HEIGHT - 1;
    cfg.pin_int = TOUCH_IRQ;
    cfg.pin_rst = TOUCH_RST;
    cfg.bus_shared = true;
    cfg.offset_rotation = 2;

    cfg.spi_host = TOUCH_SPI_HOST;
    cfg.pin_sclk = TOUCH_SCLK;
    cfg.pin_mosi = TOUCH_MOSI;
    cfg.pin_miso = TOUCH_MISO;
    cfg.pin_cs = TOUCH_CS;

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
