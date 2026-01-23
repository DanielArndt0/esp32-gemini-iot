#include "Logger.h"

namespace
{
  void printTag(const char *tag, const char *msg)
  {
    Serial.print(tag);
    Serial.println(msg);
  }

  void printTag(const char *tag, const String &msg)
  {
    Serial.print(tag);
    Serial.println(msg);
  }
}

namespace Log
{

  void begin(uint32_t baud)
  {
    Serial.begin(baud);
    delay(300);
  }

  void line()
  {
    Serial.println("--------------------------------");
  }

  void info(const char *msg) { printTag("[INFO] ", msg); }
  void warn(const char *msg) { printTag("[WARN] ", msg); }
  void error(const char *msg) { printTag("[ERR ] ", msg); }

  void info(const String &msg) { printTag("[INFO] ", msg); }
  void warn(const String &msg) { printTag("[WARN] ", msg); }
  void error(const String &msg) { printTag("[ERR ] ", msg); }

  void step(const char *name)
  {
    line();
    info(String("STEP: ") + name);
  }

  void timed(const char *label, uint32_t ms)
  {
    info(String(label) + " in " + ms + " ms");
  }

  void ok(const char *msg)
  {
    info(String("OK: ") + msg);
  }

  void bootSummary()
  {
    line();
    info("Boot summary:");
    info(String("  Baud: ") + SERIAL_BAUD);
    info(String("  OLED: SDA=") + OLED_SDA +
         " SCL=" + OLED_SCL +
         " Addr=0x" + String(OLED_I2C_ADDR_8BIT, HEX) +
         " I2C=" + String(OLED_I2C_CLOCK_HZ) + "Hz");
    info(String("  Wi-Fi: SSID=") + WIFI_SSID + " Timeout=15000ms");
    info("  Gemini: API key=(hidden)");
    line();
  }

  const char *geminiStatusText(GeminiStatus status)
  {
    switch (status)
    {
    case GeminiStatus::OK:
      return "OK";
    case GeminiStatus::RATE_LIMIT:
      return "Rate limit (429)";
    case GeminiStatus::UNAUTHORIZED:
      return "Unauthorized (401/403)";
    case GeminiStatus::NOT_FOUND:
      return "Not found (404)";
    case GeminiStatus::HTTPS_BEGIN_FAILED:
      return "HTTPS init failed";
    case GeminiStatus::CONNECTION_FAILED:
      return "Connection failed";
    case GeminiStatus::JSON_PARSE_ERROR:
      return "JSON parse error";
    case GeminiStatus::INVALID_RESPONSE:
      return "Invalid response";
    default:
      return "HTTP error";
    }
  }

  void failGemini(GeminiStatus status)
  {
    line();
    error("FAILED: Gemini request");
    error(String("Status: ") + geminiStatusText(status));

    switch (status)
    {
    case GeminiStatus::RATE_LIMIT:
      warn("Hint: wait and retry later");
      break;
    case GeminiStatus::UNAUTHORIZED:
      warn("Hint: check API key and permissions");
      break;
    case GeminiStatus::NOT_FOUND:
      warn("Hint: check endpoint or model name");
      break;
    case GeminiStatus::HTTPS_BEGIN_FAILED:
    case GeminiStatus::CONNECTION_FAILED:
      warn("Hint: check Wi-Fi, DNS and TLS");
      break;
    case GeminiStatus::JSON_PARSE_ERROR:
    case GeminiStatus::INVALID_RESPONSE:
      warn("Hint: response format may have changed");
      break;
    default:
      warn("Hint: inspect HTTP status and logs");
      break;
    }

    line();
  }

}
