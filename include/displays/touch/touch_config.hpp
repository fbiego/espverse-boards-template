#pragma once

#ifndef TOUCH_SDA
#ifdef I2C_SDA
#define TOUCH_SDA I2C_SDA
#endif
#endif

#ifndef TOUCH_SCL
#ifdef I2C_SCL
#define TOUCH_SCL I2C_SCL
#endif
#endif

#ifndef TOUCH_RST
#ifdef TP_RST
#define TOUCH_RST TP_RST
#endif
#endif

#ifndef TOUCH_IRQ
#ifdef TP_INT
#define TOUCH_IRQ TP_INT
#endif
#endif

#ifndef TOUCH_WIDTH
#ifdef TP_WIDTH
#define TOUCH_WIDTH TP_WIDTH
#else
#define TOUCH_WIDTH SCREEN_WIDTH
#endif
#endif

#ifndef TOUCH_HEIGHT
#ifdef TP_HEIGHT
#define TOUCH_HEIGHT TP_HEIGHT
#else
#define TOUCH_HEIGHT SCREEN_HEIGHT
#endif
#endif

#ifndef TOUCH_I2C_ADDR
#ifdef FT6X36_I2C_ADDR
#define TOUCH_I2C_ADDR FT6X36_I2C_ADDR
#elif defined(CST816_I2C_ADDR)
#define TOUCH_I2C_ADDR CST816_I2C_ADDR
#elif defined(CSTXXX_I2C_ADDR)
#define TOUCH_I2C_ADDR CSTXXX_I2C_ADDR
#elif defined(CHSC6540_I2C_ADDR)
#define TOUCH_I2C_ADDR CHSC6540_I2C_ADDR
#endif
#endif

#ifndef TOUCH_SPI_HOST
#ifdef TP_SPI_HOST
#define TOUCH_SPI_HOST TP_SPI_HOST
#endif
#endif

#ifndef TOUCH_SCLK
#ifdef TP_SCLK
#define TOUCH_SCLK TP_SCLK
#endif
#endif

#ifndef TOUCH_MOSI
#ifdef TP_MOSI
#define TOUCH_MOSI TP_MOSI
#endif
#endif

#ifndef TOUCH_MISO
#ifdef TP_MISO
#define TOUCH_MISO TP_MISO
#endif
#endif

#ifndef TOUCH_CS
#ifdef TP_CS
#define TOUCH_CS TP_CS
#endif
#endif
