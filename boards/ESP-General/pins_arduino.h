#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

// Some boards have too low voltage on this pin (board design bug)
// Use different pin with 3V and connect with 48
// and change this setup for the chosen pin (for example 38)
#define PIN_RGB_LED 48
// BUILTIN_LED can be used in new Arduino API digitalWrite() like in Blink.ino
static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;
#define BUILTIN_LED LED_BUILTIN  // backward compatibility
#define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN
// RGB_BUILTIN and RGB_BRIGHTNESS can be used in new Arduino API rgbLedWrite()
#define RGB_BUILTIN    LED_BUILTIN
#define RGB_BRIGHTNESS 64

static const uint8_t TX = 43;
static const uint8_t RX = 44;

static const uint8_t SDA = 8;
static const uint8_t SCL = 9;

static const uint8_t SS = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 13;
static const uint8_t SCK = 12;

static const uint8_t A0 = 1;
static const uint8_t A1 = 2;
static const uint8_t A2 = 3;
static const uint8_t A3 = 4;
static const uint8_t A4 = 5;
static const uint8_t A5 = 6;
static const uint8_t A6 = 7;
static const uint8_t A7 = 8;
static const uint8_t A8 = 9;
static const uint8_t A9 = 10;
static const uint8_t A10 = 11;
static const uint8_t A11 = 12;
static const uint8_t A12 = 13;
static const uint8_t A13 = 14;
static const uint8_t A14 = 15;
static const uint8_t A15 = 16;
static const uint8_t A16 = 17;
static const uint8_t A17 = 18;
static const uint8_t A18 = 19;
static const uint8_t A19 = 20;

static const uint8_t T1 = 1;
static const uint8_t T2 = 2;
static const uint8_t T3 = 3;
static const uint8_t T4 = 4;
static const uint8_t T5 = 5;
static const uint8_t T6 = 6;
static const uint8_t T7 = 7;
static const uint8_t T8 = 8;
static const uint8_t T9 = 9;
static const uint8_t T10 = 10;
static const uint8_t T11 = 11;
static const uint8_t T12 = 12;
static const uint8_t T13 = 13;
static const uint8_t T14 = 14;

// -----------------------------------------------------------------------------
// Bruce 1.16.1 custom display profile
// ESP32-S3-DevKitC-1 N16R8 + SSD1306 128x64 I2C
// Known-working wiring from the OLED test: SDA=GPIO8, SCL=GPIO9, address=0x3C.
// -----------------------------------------------------------------------------
#define HAS_SCREEN 1
#define USE_LOVYANGFX 1
#define LOVYAN_PANEL Panel_SSD1306
#define LOVYAN_BUS Bus_I2C
#define LOVYAN_I2C_BUS 1

#define TFT_I2C_PORT 0
#define TFT_I2C_WRITE 400000
#define TFT_I2C_READ 400000
#define TFT_SDA 8
#define TFT_SCL 9
#define TFT_ADDR 0x3C

// TFT_CS and TFT_RST are supplied as -1 by ESP-General.ini.
#define TFT_BUSY_PIN -1
#define TFT_WIDTH 128
#define TFT_HEIGHT 64
#define TFT_OFFSET_X 0
#define TFT_OFFSET_Y 0
// Invert the SSD1306 panel output: OLED background stays off/black and Bruce UI text/icons light up.
#define TFT_INVERTION 1
#define TFT_RGB_ORDER 0
#define TFT_MEM_WIDTH 128
#define TFT_MEM_HEIGHT 64

#endif /* Pins_Arduino_h */
