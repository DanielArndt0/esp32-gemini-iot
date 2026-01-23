#pragma once
#include <Arduino.h>
#include <U8g2lib.h>

class DisplayManager
{
public:
  DisplayManager();
  bool begin(int sda, int scl);

  void boot();
  void wifiStatus(const String &ssid, const String &ip, int rssiDbm);
  void working(const String &title, const String &subtitle);
  void error(const String &title, const String &details);
  void showTextPaged(const String &title, const String &text, uint32_t pageDelayMs);

private:
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C _u8g2;

  void heltecOledReset_();
  void initI2CBus_(int sda, int scl);

  void drawHeader(const String &title);
  void drawFooterPage(uint16_t page, uint16_t totalPages);
  uint16_t wrapTextToLines(const String &text, String *outLines, uint16_t maxLines, uint8_t maxPixelWidth);
};
