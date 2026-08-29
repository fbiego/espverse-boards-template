#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "displays/touch/touch_config.hpp"

#ifndef TOUCH_I2C_ADDR
#define TOUCH_I2C_ADDR 0x2E
#endif

#ifndef CHSC6540_RELEASE_GRACE_MS
#define CHSC6540_RELEASE_GRACE_MS 80
#endif

class CHSC6540Touch {
public:
  bool last_touched = false;
  uint32_t last_touch_ms = 0;
  uint16_t last_x = 0;
  uint16_t last_y = 0;

  template <typename Panel> void attach(Panel &panel) {}

  bool init(void) {
#if TOUCH_RST >= 0
    pinMode(TOUCH_RST, OUTPUT);
    digitalWrite(TOUCH_RST, LOW);
    delay(30);
    digitalWrite(TOUCH_RST, HIGH);
    delay(50);
#endif

#if TOUCH_IRQ >= 0
    pinMode(TOUCH_IRQ, INPUT_PULLUP);
#endif

    Wire.begin(TOUCH_SDA, TOUCH_SCL);
    writeRegister(0x5A, 0x5A);
    return true;
  }

  void setRotation(uint8_t rotation) {}

  template <typename Panel> bool read(Panel &panel, uint16_t *x, uint16_t *y) {
    uint8_t data[16] = {0};
    bool touched = readRegister(0x00, data, sizeof(data)) && data[2] > 0;

    if (!touched && last_touched &&
        millis() - last_touch_ms <= CHSC6540_RELEASE_GRACE_MS) {
      *x = last_x;
      *y = last_y;
      return true;
    }

    if (!touched) {
      last_touched = false;
      return false;
    }

    *x = ((uint16_t)(data[3] & 0x0F) << 8) | data[4];
    *y = ((uint16_t)(data[5] & 0x0F) << 8) | data[6];

    last_touched = true;
    last_touch_ms = millis();
    last_x = *x;
    last_y = *y;
    return true;
  }

private:
  bool writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
  }

  bool readRegister(uint8_t reg, uint8_t *data, size_t length) {
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
      return false;
    }

    size_t read = Wire.requestFrom((uint8_t)TOUCH_I2C_ADDR,
                                   (uint8_t)length);
    if (read != length) {
      return false;
    }

    for (size_t i = 0; i < length; i++) {
      data[i] = Wire.read();
    }
    return true;
  }
};
