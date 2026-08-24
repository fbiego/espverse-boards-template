#pragma once

#include "Wire.h"
#include <Arduino.h>

#define AXS5106L_ADDR 0x63
#define AXS5106L_ID_REG 0x08

#define AXS5106L_TOUCH_DATA_REG 0X01

class AXS5106LTouch {
public:
  template <typename Panel> void attach(Panel &panel) {}
  bool init(void) {
    pinMode(TP_RST, OUTPUT);

    digitalWrite(TP_RST, LOW);
    delay(200);
    digitalWrite(TP_RST, HIGH);
    delay(300);

    Wire.begin(I2C_SDA, I2C_SCL);

    return true;
  }
  void setRotation(uint8_t rotation) {
    r = rotation;
  }

  template <typename Panel> bool read(Panel &panel, uint16_t *x, uint16_t *y) {
    uint8_t data[14] = {0};
    touch_i2c_read(AXS5106L_ADDR, AXS5106L_TOUCH_DATA_REG, data, 14);
    bool touched = data[1] > 0;
    if (touched) {
      uint16_t tx = ((uint16_t)(data[2] & 0x0F) << 8) | data[3];
      uint16_t ty = ((uint16_t)(data[4] & 0x0F) << 8) | data[5];
      
      switch (r) {
      case 0: // 0°
        *x = tx;
        *y = ty;
        break;

      case 1: // 90°
        *x = SCREEN_WIDTH - 1 - ty;
        *y = tx;
        break;

      case 2: // 180°
        *x = SCREEN_WIDTH - 1 - tx;
        *y = SCREEN_HEIGHT - 1 - ty;
        break;

      case 3: // 270°
        *x = ty;
        *y = SCREEN_HEIGHT - 1 - tx;
        break;
      }
    }
    return touched;
  }

private:
  uint8_t r = 0;
  bool touch_i2c_write(uint8_t driver_addr, uint8_t reg_addr,
                       const uint8_t *data, uint32_t length) {
    Wire.beginTransmission(driver_addr);
    Wire.write(reg_addr);
    Wire.write(data, length);

    if (Wire.endTransmission() != 0) {
      return false;
    }
    return true;
  }

  bool touch_i2c_read(uint8_t driver_addr, uint8_t reg_addr, uint8_t *data,
                      uint32_t length) {
    Wire.beginTransmission(driver_addr);
    Wire.write(reg_addr);
    if (Wire.endTransmission() != 0) {
      return false;
    }

    Wire.requestFrom(driver_addr, length);
    if (Wire.available() != length) {
      return false;
    }
    Wire.readBytes(data, length);
    return true;
  }
};
