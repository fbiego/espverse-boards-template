#pragma once

/*********************
 *      DEFINES
 *********************/
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480


#define LV_BUFFER_SIZE (SCREEN_WIDTH * 100)
#define LV_BUFFER_COUNT 2


#define USE_DYNAMIC_BUFFERS 0
#define BUFFER_FLAGS -1

#define BOARD_DISPLAY_TFT_ARGS (BOARD_SC01_PLUS)


#ifndef BOARD_OEM
#define BOARD_OEM "PanelLan"
#endif
#ifndef BOARD_NAME
#define BOARD_NAME "SC01 Plus"
#endif
#ifndef DISPLAY_TYPE
#define DISPLAY_TYPE "LCD"
#endif
#define BOARD_HAS_TOUCH 1

/*********************
 *      INCLUDES
 *********************/
#include "PanelLan.h"

/*********************
 *      TYPEDEFS
 *********************/
using BoardDisplay = PanelLan;
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
