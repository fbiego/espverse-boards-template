#pragma once

/*********************
 *      DEFINES
 *********************/
// screen configs
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 296
#define OFFSET_X 0
#define OFFSET_Y 0
#define RGB_ORDER false

// touch
#define TOUCH_SDA 4
#define TOUCH_SCL 3
#define TOUCH_IRQ 2
#define TOUCH_RST 5

// display
#define TFT_SPI_HOST HSPI

#define TFT_SCLK 7
#define TFT_MOSI 8
#define TFT_MISO -1
#define TFT_DC 10
#define TFT_CS 9
#define TFT_RST 6

#define TFT_BL 13
#define LCD_EN 40
#define LCD_EN_STATE LOW
#define VIBRATION_PIN 45

#define BUZZER_PIN -1

#define LCD_IM0 47
#define LCD_IM0_STATE HIGH

#define LCD_IM1 16
#define LCD_IM1_STATE HIGH

#define LV_BUFFER_SIZE (SCREEN_WIDTH * 40)
#define LV_BUFFER_COUNT 2


#define USE_DYNAMIC_BUFFERS 0
#define BUFFER_FLAGS -1


#ifndef BOARD_OEM
#define BOARD_OEM "Elecrow"
#endif
#ifndef BOARD_NAME
#define BOARD_NAME "S3 2.01\""
#endif
#ifndef DISPLAY_TYPE
#define DISPLAY_TYPE "LCD"
#endif
#define BOARD_HAS_TOUCH 1

/*********************
 *      INCLUDES
 *********************/
#include "displays/display_wrapper.hpp"
#include "displays/panels/gc9309_spi.hpp"
#include "displays/touch/axs5106l.hpp"

#include <Arduino.h>
#include <Wire.h>

/*********************
 *      TYPEDEFS
 *********************/
using BoardDisplay =
    display::DisplayWrapper<GC9309SpiPanel, AXS5106LTouch>;
extern BoardDisplay tft;

#include "boards/common.hpp"

/*********************
 *    OTHER FUNCTIONS 
 *********************/

/*********************
 *      BOARD HOOKS
 *********************/
namespace board {
inline void before_display_init(void) {}
inline void after_display_init(void) {}
inline void after_ui_init(void) {}
inline bool wakeup_activity(void) { return false; }
inline bool screen_toggle_requested(void) { return false; }
inline void loop(void) {}
} // namespace board
