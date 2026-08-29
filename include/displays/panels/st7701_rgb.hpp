#pragma once

#define LGFX_USE_V1
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <LovyanGFX.hpp>

#include "displays/panels/lovyan_light_config.hpp"

class ST7701RGBPanel {
public:
  Arduino_DataBus *bus;
  Arduino_ESP32RGBPanel *rgbpanel;
  Arduino_RGB_Display *gfx;
  lgfx::Light_PWM light;

  ST7701RGBPanel() {
    bus = new Arduino_ESP32SPI(TFT_DC /* DC */, TFT_CS /* CS */, TFT_SCLK /* SCK */, TFT_MOSI /* MOSI */, TFT_MISO /* MISO */);
    rgbpanel = new Arduino_ESP32RGBPanel(
        ST7701_RGB_DE /* DE */, ST7701_RGB_VSYNC /* VSYNC */,
        ST7701_RGB_HSYNC /* HSYNC */, ST7701_RGB_PCLK /* PCLK */,
        ST7701_RGB_R0 /* R0 */, ST7701_RGB_R1 /* R1 */,
        ST7701_RGB_R2 /* R2 */, ST7701_RGB_R3 /* R3 */,
        ST7701_RGB_R4 /* R4 */, ST7701_RGB_G0 /* G0 */,
        ST7701_RGB_G1 /* G1 */, ST7701_RGB_G2 /* G2 */,
        ST7701_RGB_G3 /* G3 */, ST7701_RGB_G4 /* G4 */,
        ST7701_RGB_G5 /* G5 */, ST7701_RGB_B0 /* B0 */,
        ST7701_RGB_B1 /* B1 */, ST7701_RGB_B2 /* B2 */,
        ST7701_RGB_B3 /* B3 */, ST7701_RGB_B4 /* B4 */,
        ST7701_RGB_HSYNC_POLARITY /* hsync_polarity */,
        ST7701_RGB_HSYNC_FRONT_PORCH /* hsync_front_porch */,
        ST7701_RGB_HSYNC_PULSE_WIDTH /* hsync_pulse_width */,
        ST7701_RGB_HSYNC_BACK_PORCH /* hsync_back_porch */,
        ST7701_RGB_VSYNC_POLARITY /* vsync_polarity */,
        ST7701_RGB_VSYNC_FRONT_PORCH /* vsync_front_porch */,
        ST7701_RGB_VSYNC_PULSE_WIDTH /* vsync_pulse_width */,
        ST7701_RGB_VSYNC_BACK_PORCH /* vsync_back_porch */);

    gfx = new Arduino_RGB_Display(
        SCREEN_WIDTH /* width */, SCREEN_HEIGHT /* height */, rgbpanel,
        ST7701_ROTATION /* rotation */, ST7701_AUTO_FLUSH /* auto_flush */,
        bus, TFT_RST /* RST */, ST7701_INIT_OPERATIONS,
        ST7701_INIT_OPERATIONS_SIZE);

    auto cfg = light.config();
    cfg.pin_bl = TFT_BL;
    cfg.invert = TFT_BL_INVERTED;
    cfg.freq = 44100;
    cfg.pwm_channel = 1;
    light.config(cfg);
  }

  bool init(void) {
    bool state = gfx->begin();
    light.init(0);
    return state;
  }

  void initDMA(void) {}
  void fillScreen(uint16_t color) { gfx->fillScreen(color); }
  void setRotation(uint8_t rotation) {
    gfx->setRotation(rotation);
  }
  void pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data) {
    gfx->draw16bitBeRGBBitmap(x, y, data, w, h);
  }
  void pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h,
                    uint16_t *data) {
    gfx->draw16bitBeRGBBitmap(x, y, data, w, h);
  }
  void startWrite(void) {}
  uint32_t getStartCount(void) { return 0; }
  void endWrite(void) {}
  void setBrightness(uint8_t brightness) { light.setBrightness(brightness); }
  void writePixel(int32_t x, int32_t y, const uint16_t color) {
    gfx->writePixel(x, y, color);
  }
};
