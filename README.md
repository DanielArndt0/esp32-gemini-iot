# Embedded Gemini Client Demo

An embedded systems demo project using an **ESP32 (Heltec LoRa 32 V2)** with an **SSD1306 OLED display**, **Wi-Fi connectivity**, and **Google Gemini API** integration.

This project demonstrates how to bring AI-generated text into a constrained embedded environment, rendering responses directly on an OLED display with a clean, modular architecture.

---

## Features

- **ESP32 Support:** Specifically optimized for Heltec LoRa 32 V2.
- **OLED Integration:** Drivers for SSD1306 (128x64, I2C).
- **Google Gemini API:** Direct HTTPS integration for text generation.
- **Robust Connectivity:** Wi-Fi manager with connection status feedback.
- **Hardware Diagnostics:** Automated I2C bus scanning and robust bring-up sequences.
- **Modular Architecture:** Clean separation of concerns (no monolithic `main.cpp`).
- **Embedded AI Strategy:** ASCII-only output prompts and paginated text rendering for small screens.

---

## Hardware Requirements

| Component | Description |
|---------|------------|
| MCU | Heltec LoRa 32 V2 (ESP32) |
| Display | SSD1306 OLED (128x64, I2C, onboard) |
| Power | USB |
| Network | 2.4 GHz Wi-Fi |

### OLED Wiring (Heltec V2 onboard)

| Signal | GPIO |
|------|------|
| SDA | GPIO 4 |
| SCL | GPIO 15 |
| RESET | GPIO 16 |

> ⚠️ **Important:**  
> On Heltec LoRa V2, the OLED **must be reset via GPIO16** before I2C initialization.  
> Without this, the I2C bus will not detect the display.

This behavior is handled internally by `DisplayManager`.

---

## Software Requirements

- PlatformIO
- ESP32 Arduino framework
- Libraries:
  - `U8g2`
  - `ArduinoJson`

---

##  Configuration

Edit `config.h` with your settings:

```cpp
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
```

## How to Use

### Build and Upload

Using PlatformIO:

```bash
pio run -t upload
```


###  Serial Monitor

```bash
pio device monitor -b 115200
```

You should see logs like:

```bash
[INFO] Lab Embarcado - Heltec V2
[INFO] Build: Jan 23 2026 20:15:59
--------------------------------
[INFO] Boot summary:
[INFO]   Baud: 115200
[INFO]   OLED: SDA=4 SCL=15 Addr=0x78 I2C=400000Hz
[INFO]   Wi-Fi: SSID=your-wifi Timeout=15000ms
[INFO]   Gemini: API key=(hidden)
--------------------------------
--------------------------------
[INFO] STEP: Display bring-up
[INFO] Display initialized in 486 ms
--------------------------------
--------------------------------
[INFO] STEP: Wi-Fi connect
[INFO] Wi-Fi connected in 2337 ms
--------------------------------
--------------------------------
[INFO] STEP: Gemini request
[INFO] Gemini response received in 2928 ms
--------------------------------
[INFO] OK: Gemini response rendered
--------------------------------
[INFO] Done.
--------------------------------

```

## Runtime Flow

1. OLED reset and I2C bring-up  
2. Display boot screen  
3. Wi-Fi connection  
4. Status rendered on OLED  
5. Gemini prompt sent  
6. AI response paginated and rendered on display  

> No logic runs inside `loop()` (intentional design).

---

## Gemini Prompt Strategy

The project enforces **ASCII-only output**, ideal for small OLED displays.

### Example prompt behavior:
- No accents  
- No emojis  
- Short lines  
- Controlled length  

This logic is handled by `GeminiPrompts`.

---

## Troubleshooting

### OLED shows nothing
- Ensure GPIO16 reset is executed  
- Confirm SSD1306 address (`0x3C` → `0x78`)  
- Check I2C clock (100 kHz recommended)

---

### I2C scan finds nothing
- This is expected **before OLED reset**
- Use `I2CDiagnostic` for validation

---

### Gemini returns errors
- Check API key  
- Watch for rate limits (HTTP 429)  
- Serial logs provide decoded error reasons

---

## Author

**Daniel Gier Arndt**
Embedded systems, electronics and applied AI  
YouTube: [*@LabEmbarcado*](https://www.youtube.com/@LabEmbarcado)

---

## Final Notes

This is **not a tutorial-style repository**.  
It is a **clean, practical demo** focused on real embedded constraints and solutions.
