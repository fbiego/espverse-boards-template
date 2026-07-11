/*
   MIT License

  Copyright (c) 2025 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ______________  _____
  ___  __/___  /_ ___(_)_____ _______ _______
  __  /_  __  __ \__  / _  _ \__  __ `/_  __ \
  _  __/  _  /_/ /_  /  /  __/_  /_/ / / /_/ /
  /_/     /_.___/ /_/   \___/ _\__, /  \____/
                              /____/

*/

#include <Arduino.h>
#include <Preferences.h>
#include <cstdint>
#include <stdlib.h>
#include <timber.h>

#include "display/lvgl_port.hpp"
#include "globals.hpp"
#include "main.h"

Preferences prefs;

static void set_screen_brightness_level(uint8_t value) {
#if BOARD_HAS_CUSTOM_BRIGHTNESS == 1
  board::set_brightness(value);
#else
  tft.setBrightness(value);
#endif
}

lv_obj_t *label;
lv_obj_t *dot;
lv_obj_t *button;

void touch_test_screen_event_cb(lv_event_t *e) {
  lv_event_code_t event_code = lv_event_get_code(e);

  if (event_code == LV_EVENT_PRESSING) {
    /* Do something when the screen is pressed */
    /* You can get the coordinates of the press with
     * `lv_indev_get_point(lv_indev_active(), &point);` */
    lv_point_t point;
    lv_indev_get_point(lv_indev_active(), &point);
    lv_obj_set_pos(dot, point.x - 10, point.y - 10);
    lv_obj_set_flag(dot, LV_OBJ_FLAG_HIDDEN, false);
#ifdef ESP32_CYD
    lv_obj_set_flag(button, LV_OBJ_FLAG_HIDDEN, true);
#endif

    lv_label_set_text_fmt(label, "Touched at (%d, %d)", point.x, point.y);
  }
  if (event_code == LV_EVENT_RELEASED) {
    /* Do something when the screen is released */
    lv_obj_set_flag(dot, LV_OBJ_FLAG_HIDDEN, true);
#ifdef ESP32_CYD
    lv_obj_set_flag(button, LV_OBJ_FLAG_HIDDEN, false);
#endif

    lv_label_set_text(label, "Released");
  }
}

void __attribute__((weak)) clear_touch_calibration(void) {
  lv_label_set_text(label, "Function not implemented.");
}

void button_event_cb(lv_event_t *e) {
  lv_event_code_t event_code = lv_event_get_code(e);
  if (event_code == LV_EVENT_CLICKED) {
    /* Do something when the button is clicked */
    /* This is a good place to start an action or open a new screen */
    lv_label_set_text(label, "Restarting");
    // Add your recalibration code here
    clear_touch_calibration();
  }
}

lv_obj_t *touch_test_screen_init(void) {
  /* create the screen */
  lv_obj_t *touch_test_screen_main = lv_obj_create(NULL);
  lv_obj_remove_flag(touch_test_screen_main, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(touch_test_screen_main, lv_color_hex(0x555555),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(touch_test_screen_main, 255,
                          LV_PART_MAIN | LV_STATE_DEFAULT);

  label = lv_label_create(touch_test_screen_main);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_label_set_text(label, "Hello world!\nTouch Test App");

  dot = lv_obj_create(touch_test_screen_main);
  lv_obj_remove_style_all(dot);
  lv_obj_set_size(dot, 20, 20);
  lv_obj_set_style_bg_color(dot, lv_color_hex(0xFFFFFF),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(dot, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE,
                          LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(dot, LV_ALIGN_TOP_LEFT);
  lv_obj_add_flag(dot, LV_OBJ_FLAG_HIDDEN);
  lv_obj_remove_flag(dot, LV_OBJ_FLAG_CLICKABLE);

#ifdef ESP32_CYD
  button = lv_button_create(touch_test_screen_main);
  lv_obj_set_align(button, LV_ALIGN_CENTER);
  lv_obj_set_y(button, 100);
  lv_obj_set_size(button, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_t *btn_label = lv_label_create(button);
  lv_label_set_text(btn_label, "Recalibrate");
  lv_obj_center(btn_label);
  lv_obj_add_event_cb(button, button_event_cb, LV_EVENT_CLICKED, NULL);
#endif

  lv_obj_add_event_cb(touch_test_screen_main, touch_test_screen_event_cb,
                      LV_EVENT_ALL, NULL);

  return touch_test_screen_main;
}

void setup() {

  Serial.begin(115200);

  prefs.begin("my-app");

  board::before_display_init();

  tft.init();
  tft.initDMA();
  tft.startWrite();
  tft.fillScreen(0x0000);
  set_screen_brightness_level(255);
  board::after_display_init();

#if BOARD_ROTATION_LOCKED == 1 && BOARD_ROTATION_VALUE >= 0
  rotation = BOARD_ROTATION_VALUE;
#endif

  lvgl_port_init();

  lv_screen_load(touch_test_screen_init());

  board::after_ui_init();

  Serial.println("Setup complete");
}

void loop() {
  lv_timer_handler(); // Update the UI-
  delay(5);
  board::loop();
}
