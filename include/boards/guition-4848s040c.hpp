#pragma once

/*********************
 *      DEFINES
 *********************/
// screen configs
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

// touch
#define TOUCH_SDA 19
#define TOUCH_SCL 45
#define TOUCH_IRQ -1
#define TOUCH_RST -1
#define TOUCH_WIDTH 480
#define TOUCH_HEIGHT 480

// display
#define TFT_SPI_HOST VSPI_HOST

#define TFT_SCLK 48
#define TFT_MOSI 47
#define TFT_MISO -1
#define TFT_DC -1
#define TFT_CS 39
#define TFT_RST -1

#define TFT_BL 38

#define ST7701_RGB_DE 18
#define ST7701_RGB_VSYNC 17
#define ST7701_RGB_HSYNC 16
#define ST7701_RGB_PCLK 21
#define ST7701_RGB_R0 11
#define ST7701_RGB_R1 12
#define ST7701_RGB_R2 13
#define ST7701_RGB_R3 14
#define ST7701_RGB_R4 0
#define ST7701_RGB_G0 8
#define ST7701_RGB_G1 20
#define ST7701_RGB_G2 3
#define ST7701_RGB_G3 46
#define ST7701_RGB_G4 9
#define ST7701_RGB_G5 10
#define ST7701_RGB_B0 4
#define ST7701_RGB_B1 5
#define ST7701_RGB_B2 6
#define ST7701_RGB_B3 7
#define ST7701_RGB_B4 15

#define ST7701_RGB_HSYNC_POLARITY 1
#define ST7701_RGB_HSYNC_FRONT_PORCH 10
#define ST7701_RGB_HSYNC_PULSE_WIDTH 8
#define ST7701_RGB_HSYNC_BACK_PORCH 50
#define ST7701_RGB_VSYNC_POLARITY 1
#define ST7701_RGB_VSYNC_FRONT_PORCH 10
#define ST7701_RGB_VSYNC_PULSE_WIDTH 8
#define ST7701_RGB_VSYNC_BACK_PORCH 20

#define ST7701_ROTATION 0
#define ST7701_AUTO_FLUSH true
#define TAMC_GT911_ROTATION ROTATION_INVERTED

#define LV_BUFFER_SIZE (SCREEN_WIDTH * 100)
#define LV_BUFFER_COUNT 2


#define USE_DYNAMIC_BUFFERS 0
#define BUFFER_FLAGS -1

#define BOARD_OEM "Guition"
#define BOARD_NAME "4848S040C"
#define DISPLAY_TYPE "LCD"
#define BOARD_HAS_TOUCH 1

#include <Arduino_GFX_Library.h>

static const uint8_t st7701_4848s040c_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x10,

    WRITE_C8_D16, 0xC0, 0x3B, 0x00,
    WRITE_C8_D16, 0xC1, 0x0D, 0x02,
    WRITE_C8_D16, 0xC2, 0x31, 0x05,
    WRITE_C8_D8, 0xCD, 0x00, // 0x08 /* Changed by manufacturer */

    WRITE_COMMAND_8, 0xB0, // Positive Voltage Gamma Control
    WRITE_BYTES, 16,
    0x00, 0x11, 0x18, 0x0E,
    0x11, 0x06, 0x07, 0x08,
    0x07, 0x22, 0x04, 0x12,
    0x0F, 0xAA, 0x31, 0x18,

    WRITE_COMMAND_8, 0xB1, // Negative Voltage Gamma Control
    WRITE_BYTES, 16,
    0x00, 0x11, 0x19, 0x0E,
    0x12, 0x07, 0x08, 0x08,
    0x08, 0x22, 0x04, 0x11,
    0x11, 0xA9, 0x32, 0x18,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x11,

    WRITE_C8_D8, 0xB0, 0x60,
    WRITE_C8_D8, 0xB1, 0x32,
    WRITE_C8_D8, 0xB2, 0x07,
    WRITE_C8_D8, 0xB3, 0x80,
    WRITE_C8_D8, 0xB5, 0x49,
    WRITE_C8_D8, 0xB7, 0x85,
    WRITE_C8_D8, 0xB8, 0x21,
    WRITE_C8_D8, 0xC1, 0x78,
    WRITE_C8_D8, 0xC2, 0x78,

    WRITE_COMMAND_8, 0xE0,
    WRITE_BYTES, 3, 0x00, 0x1B, 0x02,

    WRITE_COMMAND_8, 0xE1,
    WRITE_BYTES, 11,
    0x08, 0xA0, 0x00, 0x00,
    0x07, 0xA0, 0x00, 0x00,
    0x00, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE2,
    WRITE_BYTES, 12,
    0x11, 0x11, 0x44, 0x44,
    0xED, 0xA0, 0x00, 0x00,
    0xEC, 0xA0, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE3,
    WRITE_BYTES, 4, 0x00, 0x00, 0x11, 0x11,

    WRITE_C8_D16, 0xE4, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE5,
    WRITE_BYTES, 16,
    0x0A, 0xE9, 0xD8, 0xA0,
    0x0C, 0xEB, 0xD8, 0xA0,
    0x0E, 0xED, 0xD8, 0xA0,
    0x10, 0xEF, 0xD8, 0xA0,

    WRITE_COMMAND_8, 0xE6,
    WRITE_BYTES, 4, 0x00, 0x00, 0x11, 0x11,

    WRITE_C8_D16, 0xE7, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 16,
    0x09, 0xE8, 0xD8, 0xA0,
    0x0B, 0xEA, 0xD8, 0xA0,
    0x0D, 0xEC, 0xD8, 0xA0,
    0x0F, 0xEE, 0xD8, 0xA0,

    WRITE_COMMAND_8, 0xEB,
    WRITE_BYTES, 7,
    0x02, 0x00, 0xE4, 0xE4,
    0x88, 0x00, 0x40,

    WRITE_C8_D16, 0xEC, 0x3C, 0x00,

    WRITE_COMMAND_8, 0xED,
    WRITE_BYTES, 16,
    0xAB, 0x89, 0x76, 0x54,
    0x02, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x20,
    0x45, 0x67, 0x98, 0xBA,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x13,

    WRITE_C8_D8, 0xE5, 0xE4,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x00,

    WRITE_C8_D8, 0x3A, 0x60,

    WRITE_COMMAND_8, 0x11,
    END_WRITE,

    DELAY, 120,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x29,
    END_WRITE};

#define ST7701_INIT_OPERATIONS st7701_4848s040c_init_operations
#define ST7701_INIT_OPERATIONS_SIZE sizeof(st7701_4848s040c_init_operations)

/*********************
 *      INCLUDES
 *********************/
#include "displays/display_wrapper.hpp"
#include "displays/panels/st7701_rgb.hpp"
#include "displays/touch/tamc_gt911.hpp"

/*********************
 *      TYPEDEFS
 *********************/
using BoardDisplay =
    display::DisplayWrapper<ST7701RGBPanel, GT911Touch>;
extern BoardDisplay tft;

#include "boards/common.hpp"

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
