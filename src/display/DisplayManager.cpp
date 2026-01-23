#include "DisplayManager.h"
#include "config.h"
#include <Wire.h>

namespace
{
  constexpr uint8_t W = 128;
  constexpr uint8_t H = 64;

  constexpr uint8_t HEADER_H = 14;
  constexpr uint8_t FOOTER_H = 10;

  constexpr uint8_t BODY_Y = HEADER_H + 2;
  constexpr uint8_t BODY_H = H - HEADER_H - FOOTER_H - 2;

  constexpr uint8_t LINE_H = 11;
  constexpr uint8_t MAX_BODY_LINES = BODY_H / LINE_H;
}

DisplayManager::DisplayManager()
    : _u8g2(U8G2_R0, U8X8_PIN_NONE)
{
}

void DisplayManager::heltecOledReset_()
{
  pinMode(OLED_RESET, OUTPUT);
  digitalWrite(OLED_RESET, LOW);
  delay(50);
  digitalWrite(OLED_RESET, HIGH);
  delay(50);
}

void DisplayManager::initI2CBus_(int sda, int scl)
{
  Wire.begin(sda, scl);
  Wire.setClock(OLED_I2C_CLOCK_HZ);
  delay(10);
}

bool DisplayManager::begin(int sda, int scl)
{
  heltecOledReset_();
  initI2CBus_(sda, scl);

  _u8g2.begin();

  _u8g2.setI2CAddress(OLED_I2C_ADDR_8BIT);

  _u8g2.setFont(u8g2_font_6x10_tf);
  _u8g2.setFontMode(1);
  _u8g2.setDrawColor(1);

  boot();
  return true;
}

void DisplayManager::drawHeader(const String &title)
{
  _u8g2.setDrawColor(1);
  _u8g2.drawBox(0, 0, W, HEADER_H);

  _u8g2.setDrawColor(0);
  _u8g2.setFont(u8g2_font_6x10_tf);
  _u8g2.setCursor(4, 11);
  _u8g2.print(title);

  _u8g2.setDrawColor(1);
}

void DisplayManager::drawFooterPage(uint16_t page, uint16_t totalPages)
{
  _u8g2.drawHLine(0, H - FOOTER_H, W);

  _u8g2.setFont(u8g2_font_5x8_tf);
  _u8g2.setCursor(4, H - 2);
  _u8g2.print("Lab Embarcado");

  String p = String(page) + "/" + String(totalPages);
  int pw = _u8g2.getStrWidth(p.c_str());
  _u8g2.setCursor(W - pw - 4, H - 2);
  _u8g2.print(p);
}

void DisplayManager::boot()
{
  _u8g2.clearBuffer();
  drawHeader("Boot");

  _u8g2.setFont(u8g2_font_6x10_tf);
  _u8g2.setCursor(0, BODY_Y + 10);
  _u8g2.print("Heltec LoRa V2");

  _u8g2.setCursor(0, BODY_Y + 24);
  _u8g2.print("OLED SSD1306");

  _u8g2.setCursor(0, BODY_Y + 38);
  _u8g2.print("Ready");

  drawFooterPage(1, 1);
  _u8g2.sendBuffer();
}

void DisplayManager::wifiStatus(const String &ssid, const String &ip, int rssiDbm)
{
  _u8g2.clearBuffer();
  drawHeader("Wi-Fi");

  _u8g2.setFont(u8g2_font_6x10_tf);

  _u8g2.setCursor(0, BODY_Y + 10);
  _u8g2.print("SSID: ");
  _u8g2.print(ssid);

  _u8g2.setCursor(0, BODY_Y + 24);
  _u8g2.print("IP: ");
  _u8g2.print(ip);

  _u8g2.setCursor(0, BODY_Y + 38);
  _u8g2.print("RSSI: ");
  _u8g2.print(rssiDbm);
  _u8g2.print(" dBm");

  drawFooterPage(1, 1);
  _u8g2.sendBuffer();
}

void DisplayManager::working(const String &title, const String &subtitle)
{
  _u8g2.clearBuffer();
  drawHeader(title);

  _u8g2.setFont(u8g2_font_6x10_tf);
  _u8g2.setCursor(0, BODY_Y + 14);
  _u8g2.print(subtitle);

  _u8g2.setFont(u8g2_font_5x8_tf);
  _u8g2.setCursor(0, BODY_Y + 34);
  _u8g2.print("Wait...");

  drawFooterPage(1, 1);
  _u8g2.sendBuffer();
}

void DisplayManager::error(const String &title, const String &details)
{
  _u8g2.clearBuffer();
  drawHeader("Error");

  _u8g2.setFont(u8g2_font_6x10_tf);
  _u8g2.setCursor(0, BODY_Y + 14);
  _u8g2.print(title);

  _u8g2.setFont(u8g2_font_5x8_tf);
  _u8g2.setCursor(0, BODY_Y + 30);
  _u8g2.print(details);

  drawFooterPage(1, 1);
  _u8g2.sendBuffer();
}

uint16_t DisplayManager::wrapTextToLines(
    const String &text,
    String *outLines,
    uint16_t maxLines,
    uint8_t maxPixelWidth)
{
  _u8g2.setFont(u8g2_font_6x10_tf);

  uint16_t lineCount = 0;
  String line;
  String word;

  auto flushLine = [&]()
  {
    if (lineCount < maxLines && line.length() > 0)
    {
      outLines[lineCount++] = line;
      line = "";
    }
  };

  for (uint16_t i = 0; i <= text.length(); i++)
  {
    char c = (i < text.length()) ? text[i] : ' ';
    bool isBreak = (c == ' ' || c == '\n' || c == '\r' || i == text.length());

    if (!isBreak)
    {
      word += c;
      continue;
    }

    if (word.length() == 0)
    {
      if (c == '\n')
        flushLine();
      continue;
    }

    String candidate = line;
    if (candidate.length() > 0)
      candidate += ' ';
    candidate += word;

    if (_u8g2.getStrWidth(candidate.c_str()) <= maxPixelWidth)
    {
      line = candidate;
    }
    else
    {
      flushLine();
      if (_u8g2.getStrWidth(word.c_str()) <= maxPixelWidth)
      {
        line = word;
      }
      else
      {
        String chunk;
        for (uint16_t k = 0; k < word.length(); k++)
        {
          String chunkCandidate = chunk + word[k];
          if (_u8g2.getStrWidth(chunkCandidate.c_str()) <= maxPixelWidth)
          {
            chunk = chunkCandidate;
          }
          else
          {
            if (lineCount < maxLines)
              outLines[lineCount++] = chunk;
            chunk = String(word[k]);
            if (lineCount >= maxLines)
              break;
          }
        }
        line = chunk;
      }
    }

    word = "";
    if (c == '\n')
      flushLine();
    if (lineCount >= maxLines)
      break;
  }

  flushLine();
  return lineCount;
}

void DisplayManager::showTextPaged(const String &title, const String &text, uint32_t pageDelayMs)
{
  constexpr uint16_t MAX_LINES = 64;
  String lines[MAX_LINES];

  const uint8_t maxWidth = W - 4;

  const uint16_t totalLines = wrapTextToLines(text, lines, MAX_LINES, maxWidth);
  const uint8_t linesPerPage = (MAX_BODY_LINES > 0) ? MAX_BODY_LINES : 3;

  const uint16_t totalPages = (totalLines == 0)
                                  ? 1
                                  : (uint16_t)((totalLines + linesPerPage - 1) / linesPerPage);

  for (uint16_t page = 1; page <= totalPages; page++)
  {
    _u8g2.clearBuffer();
    drawHeader(title);

    _u8g2.setFont(u8g2_font_6x10_tf);

    uint16_t start = (page - 1) * linesPerPage;
    for (uint8_t i = 0; i < linesPerPage; i++)
    {
      uint16_t idx = start + i;
      if (idx >= totalLines)
        break;

      uint8_t y = BODY_Y + 10 + (i * LINE_H);
      _u8g2.setCursor(2, y);
      _u8g2.print(lines[idx]);
    }

    drawFooterPage(page, totalPages);
    _u8g2.sendBuffer();

    if (page < totalPages)
      delay(pageDelayMs);
  }
}
