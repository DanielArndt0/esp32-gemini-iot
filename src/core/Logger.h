#pragma once
#include <Arduino.h>
#include "config.h"
#include "ai/GeminiClient.h"

namespace Log {

  void begin(uint32_t baud);
  void line();

  void info(const char* msg);
  void warn(const char* msg);
  void error(const char* msg);

  void info(const String& msg);
  void warn(const String& msg);
  void error(const String& msg);

  void step(const char* name);
  void timed(const char* label, uint32_t ms);
  void ok(const char* msg);

  void bootSummary();

  const char* geminiStatusText(GeminiStatus status);
  void failGemini(GeminiStatus status);

}
