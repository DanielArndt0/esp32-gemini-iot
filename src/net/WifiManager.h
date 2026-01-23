#pragma once
#include <Arduino.h>

class WifiManager
{
public:
  void init();
  bool connect(const char *ssid, const char *pass, uint32_t timeoutMs);

  bool connected() const;
  String ip() const;
  int rssi() const;
};
