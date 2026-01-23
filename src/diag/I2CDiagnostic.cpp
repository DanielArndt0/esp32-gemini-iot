#include "I2CDiagnostic.h"
#include <Wire.h>
#include "core/Logger.h"

namespace I2CDiagnostic
{

  static void add(Result &r, uint8_t addr)
  {
    if (r.count < sizeof(r.addrs))
    {
      r.addrs[r.count++] = addr;
    }
  }

  void beginBus(int sda, int scl, bool useReset, int resetPin)
  {
    if (useReset && resetPin >= 0)
    {
      pinMode(resetPin, OUTPUT);
      digitalWrite(resetPin, LOW);
      delay(50);
      digitalWrite(resetPin, HIGH);
      delay(50);
    }

    Wire.begin(sda, scl);
    delay(10);

    Log::info(String("[I2C] Wire.begin SDA=") + sda + " SCL=" + scl +
              (useReset ? " (OLED reset ok)" : " (no reset)"));
  }

  Result scanAt(uint32_t clockHz)
  {
    Result r;
    r.clockHz = clockHz;

    Wire.setClock(clockHz);
    delay(20);

    for (uint8_t addr = 1; addr < 127; addr++)
    {
      Wire.beginTransmission(addr);
      uint8_t err = Wire.endTransmission();

      if (err == 0)
        add(r, addr);
      delay(1);
    }

    return r;
  }

  void printResult(const Result &r)
  {
    String head = String("[I2C] @") + r.clockHz + " Hz -> " + r.count + " device(s)";
    if (r.count == 0)
    {
      Log::warn(head);
      return;
    }

    Log::info(head);

    bool has3C = false, has3D = false;

    for (uint8_t i = 0; i < r.count; i++)
    {
      char buf[40];
      snprintf(buf, sizeof(buf), "   found: 0x%02X", r.addrs[i]);
      Log::info(buf);

      if (r.addrs[i] == 0x3C)
        has3C = true;
      if (r.addrs[i] == 0x3D)
        has3D = true;
    }

    if (has3C)
      Log::info("   OLED? 0x3C (U8g2 addr 0x78)");
    if (has3D)
      Log::info("   OLED? 0x3D (U8g2 addr 0x7A)");
  }

  void runMultiFreq(const uint32_t *clocks, size_t nClocks)
  {
    Log::line();
    Log::info("[I2C] Multi-frequency scan");

    for (size_t i = 0; i < nClocks; i++)
    {
      Result r = scanAt(clocks[i]);
      printResult(r);
    }

    Log::line();
  }

}
