#pragma once
#include <Arduino.h>

enum class GeminiStatus {
  OK,
  HTTPS_BEGIN_FAILED,
  CONNECTION_FAILED,
  UNAUTHORIZED,
  NOT_FOUND,
  RATE_LIMIT,
  HTTP_ERROR,
  JSON_PARSE_ERROR,
  INVALID_RESPONSE
};

class GeminiClient {
public:
  GeminiStatus generateText(const String& prompt, String& outText);

private:
  bool buildPayload(const String& prompt, String& out);
  GeminiStatus postRequest(const String& payload, String& response);
  GeminiStatus parseResponse(const String& response, String& outText);
};
