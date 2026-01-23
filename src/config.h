#pragma once

// Serial
#define SERIAL_BAUD 115200

// Wifi
#define WIFI_SSID "YOUR SSID"
#define WIFI_PASS "YOUR PASSWORD"

// OLED
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RESET 16

// OLED I2C
#define OLED_I2C_CLOCK_HZ 100E3
#define OLED_I2C_ADDR_8BIT 0x78

// UI
#define OLED_PAGE_DELAY_MS 2500

// Gemini
static const char *GEMINI_API_KEY = "YOUR GEMINI API KEY";
static const char *GEMINI_ENDPOINT =
    "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash-lite:generateContent";