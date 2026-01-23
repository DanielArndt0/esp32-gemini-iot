#pragma once
#include <Arduino.h>

namespace I2CDiagnostic
{

  struct Result
  {
    uint32_t clockHz = 0;
    uint8_t addrs[16];
    uint8_t count = 0;
  };

  void beginBus(int sda, int scl, bool useReset, int resetPin);
  Result scanAt(uint32_t clockHz);
  void printResult(const Result &r);
  void runMultiFreq(const uint32_t *clocks, size_t nClocks);

}
