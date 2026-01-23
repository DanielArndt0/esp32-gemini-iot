#include "GeminiClient.h"
#include "config.h"

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


GeminiStatus GeminiClient::generateText(const String& prompt, String& outText) {
  String payload;
  if (!buildPayload(prompt, payload)) {
    return GeminiStatus::INVALID_RESPONSE;
  }

  String response;
  GeminiStatus status = postRequest(payload, response);
  if (status != GeminiStatus::OK) {
    return status;
  }

  return parseResponse(response, outText);
}

bool GeminiClient::buildPayload(const String& prompt, String& out) {
  JsonDocument doc;

  JsonArray contents = doc["contents"].to<JsonArray>();
  JsonObject content = contents.add<JsonObject>();
  JsonArray parts = content["parts"].to<JsonArray>();

  JsonObject textPart = parts.add<JsonObject>();
  textPart["text"] = prompt;

  out.clear();
  serializeJson(doc, out);
  return true;
}

GeminiStatus GeminiClient::postRequest(const String& payload, String& response) {
  WiFiClientSecure client;
  client.setInsecure(); 

  HTTPClient https;
  String url = String(GEMINI_ENDPOINT) + "?key=" + GEMINI_API_KEY;

  if (!https.begin(client, url)) {
    return GeminiStatus::HTTPS_BEGIN_FAILED;
  }

  https.addHeader("Content-Type", "application/json");

  const int httpCode = https.POST(payload);

  if (httpCode <= 0) {
    https.end();
    return GeminiStatus::CONNECTION_FAILED;
  }

  response = https.getString();
  https.end();

  switch (httpCode) {
    case 200:
      return GeminiStatus::OK;

    case 401:
    case 403:
      return GeminiStatus::UNAUTHORIZED;

    case 404:
      return GeminiStatus::NOT_FOUND;

    case 429:
      return GeminiStatus::RATE_LIMIT;

    default:
      return GeminiStatus::HTTP_ERROR;
  }
}


GeminiStatus GeminiClient::parseResponse(const String& response, String& outText) {
  JsonDocument doc;

  DeserializationError err = deserializeJson(doc, response);
  if (err) {
    return GeminiStatus::JSON_PARSE_ERROR;
  }

  const char* text =
      doc["candidates"][0]["content"]["parts"][0]["text"].as<const char*>();

  if (!text || text[0] == '\0') {
    return GeminiStatus::INVALID_RESPONSE;
  }

  outText = String(text);
  return GeminiStatus::OK;
}
