#pragma once
#include <Arduino.h>

#include "ai/GeminiClient.h"

class GeminiService
{
public:
  explicit GeminiService(GeminiClient &client);

  GeminiStatus generateWithRetry(const String &prompt, String &outText);

  void setMaxAttempts(int v) { _maxAttempts = v; }
  void setInitialBackoffMs(uint32_t v) { _initialBackoffMs = v; }

private:
  GeminiClient &_client;
  int _maxAttempts = 4;
  uint32_t _initialBackoffMs = 1000;
};
