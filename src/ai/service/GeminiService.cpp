#include "GeminiService.h"

GeminiService::GeminiService(GeminiClient &client)
    : _client(client)
{
}

GeminiStatus GeminiService::generateWithRetry(const String &prompt, String &outText)
{
  GeminiStatus status = GeminiStatus::HTTP_ERROR;
  uint32_t backoff = _initialBackoffMs;

  for (int attempt = 1; attempt <= _maxAttempts; ++attempt)
  {
    status = _client.generateText(prompt, outText);

    if (status == GeminiStatus::OK)
      return status;

    if (status == GeminiStatus::RATE_LIMIT)
    {
      delay(backoff);
      backoff *= 2;
      continue;
    }

    break;
  }

  return status;
}
