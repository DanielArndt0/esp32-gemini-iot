#include "WifiManager.h"
#include <WiFi.h>

void WifiManager::init()
{
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);
}

bool WifiManager::connect(const char *ssid, const char *pass, uint32_t timeoutMs)
{
  WiFi.disconnect(true);
  delay(200);

  WiFi.begin(ssid, pass);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    if (millis() - start > timeoutMs)
      return false;
  }
  return true;
}

bool WifiManager::connected() const
{
  return WiFi.status() == WL_CONNECTED;
}

String WifiManager::ip() const
{
  return connected() ? WiFi.localIP().toString() : "-";
}

int WifiManager::rssi() const
{
  return connected() ? WiFi.RSSI() : 0;
}
