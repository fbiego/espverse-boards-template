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

void espverse_home_screen_create(void);

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

void clear_touch_calibration(void) {
  prefs.remove("touch_calib");
  ESP.restart();
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
  tft.setRotation(BOARD_ROTATION_VALUE);
#endif

  lvgl_port_init();

  // Load the touch test screen
  lv_screen_load(touch_test_screen_init());

  espverse_home_screen_create();

  board::after_ui_init();

  Serial.println("Setup complete");
}

void loop() {
  lv_timer_handler(); // Update the UI-
  delay(5);
  board::loop();
}


#ifndef ESPVERSE_BOARD_ID
#define ESPVERSE_BOARD_ID "esp32s3-board"
#endif

#ifndef ESPVERSE_MANUFACTURER
#define ESPVERSE_MANUFACTURER "ESPVerse"
#endif

#ifndef ESPVERSE_BOARD_NAME
#define ESPVERSE_BOARD_NAME "Board Template"
#endif

#ifndef ESPVERSE_DISPLAY_RES
#define ESPVERSE_DISPLAY_RES "320 x 240"
#endif

#ifndef ESPVERSE_FW_VERSION
#define ESPVERSE_FW_VERSION "0.1.0"
#endif

static void touch_test_cb(lv_event_t * e) {
    LV_UNUSED(e);
    /* open touch test screen */
}

static void rotation_test_cb(lv_event_t * e) {
    LV_UNUSED(e);
    /* cycle display rotation */
}

static void calibrate_cb(lv_event_t * e) {
    LV_UNUSED(e);
    /* open calibration flow */
}

static lv_obj_t * make_value_row(lv_obj_t * parent, const char * label, const char * value) {
    lv_obj_t * row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * label_obj = lv_label_create(row);
    lv_label_set_text(label_obj, label);
    lv_obj_set_style_text_color(label_obj, lv_color_hex(0xa8b7c8), 0);

    lv_obj_t * value_obj = lv_label_create(row);
    lv_label_set_text(value_obj, value);
    lv_obj_set_style_text_color(value_obj, lv_color_hex(0xedf6ff), 0);
    lv_obj_set_style_text_align(value_obj, LV_TEXT_ALIGN_RIGHT, 0);

    return row;
}

static lv_obj_t * make_button(lv_obj_t * parent, const char * text, lv_event_cb_t cb) {
    lv_obj_t * btn = lv_button_create(parent);
    lv_obj_set_height(btn, 40);
    lv_obj_set_flex_grow(btn, 1);
    lv_obj_set_style_radius(btn, 8, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x101721), 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x314253), 0);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xedf6ff), 0);
    lv_obj_center(label);

    return btn;
}

void espverse_home_screen_create(void) {
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x07090d), 0);
    lv_obj_set_style_pad_all(screen, 10, 0);

    lv_obj_t * root = lv_obj_create(screen);
    lv_obj_remove_style_all(root);
    lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(root, 8, 0);

    lv_obj_t * header = lv_obj_create(root);
    lv_obj_remove_style_all(header);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_height(header, 34);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(header);
    lv_label_set_text(title, "ESPVerse");
    lv_obj_set_style_text_color(title, lv_color_hex(0xedf6ff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);

    lv_obj_t * status = lv_label_create(header);
    lv_label_set_text(status, "READY");
    lv_obj_set_style_text_color(status, lv_color_hex(0x6ee7a8), 0);

    lv_obj_t * panel = lv_obj_create(root);
    lv_obj_set_width(panel, LV_PCT(100));
    lv_obj_set_flex_grow(panel, 1);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x101721), 0);
    lv_obj_set_style_radius(panel, 8, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x243140), 0);
    lv_obj_set_style_pad_all(panel, 10, 0);
    lv_obj_set_style_pad_row(panel, 6, 0);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);

    make_value_row(panel, "Board ID", ESPVERSE_BOARD_ID);
    make_value_row(panel, "Manufacturer", ESPVERSE_MANUFACTURER);
    make_value_row(panel, "Board", ESPVERSE_BOARD_NAME);
    make_value_row(panel, "Display", ESPVERSE_DISPLAY_RES);
    make_value_row(panel, "LVGL", LVGL_VERSION_INFO);
    make_value_row(panel, "Firmware", ESPVERSE_FW_VERSION);

    lv_obj_t * actions = lv_obj_create(root);
    lv_obj_remove_style_all(actions);
    lv_obj_set_width(actions, LV_PCT(100));
    lv_obj_set_height(actions, 44);
    lv_obj_set_flex_flow(actions, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(actions, 6, 0);

    make_button(actions, "Touch", touch_test_cb);
    make_button(actions, "Rotate", rotation_test_cb);
    make_button(actions, "Cal", calibrate_cb);

    lv_screen_load(screen);
}