#include <Arduino.h>

#include "config.h"
#include "core/Logger.h"

#include "net/WifiManager.h"
#include "display/DisplayManager.h"

#include "ai/GeminiClient.h"
#include "ai/service/GeminiService.h"
#include "ai/prompts/GeminiPrompts.h"

namespace
{
  WifiManager wifi;
  DisplayManager display;

  GeminiClient geminiClient;
  GeminiService gemini(geminiClient);

}

void setup()
{
  Log::begin(SERIAL_BAUD);

  Log::info("Lab Embarcado - Heltec V2");
  Log::info(String("Build: ") + __DATE__ + " " + __TIME__);
  Log::bootSummary();

  Log::step("Display bring-up");
  uint32_t t0 = millis();
  if (!display.begin(OLED_SDA, OLED_SCL))
  {
    display.error("Display", "Init failed");
    return;
  }
  Log::timed("Display initialized", millis() - t0);

  Log::step("Wi-Fi connect");
  display.working("Wi-Fi", "Connecting...");
  t0 = millis();
  if (!wifi.connect(WIFI_SSID, WIFI_PASS, 15000))
  {
    display.error("Wi-Fi", "Timeout");
    return;
  }
  Log::timed("Wi-Fi connected", millis() - t0);

  Log::step("Gemini request");
  display.working("Gemini", "Generating...");

  const String prompt =
      GeminiPrompts::asciiOnlyPtBr(
          "Explain I2C in embedded systems in a simple way",
          6);

  String answer;
  t0 = millis();
  GeminiStatus st = gemini.generateWithRetry(prompt, answer);
  Log::timed("Gemini response received", millis() - t0);

  if (st != GeminiStatus::OK)
  {
    Log::failGemini(st);
    display.error("Gemini", "Request failed");
    return;
  }

  Log::ok("Gemini response rendered");
  display.showTextPaged("Gemini", answer, OLED_PAGE_DELAY_MS);

  Log::line();
  Log::info("Done.");
  Log::line();
}

void loop()
{
}
