#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <deque>
namespace esphome {
inline uint32_t test_now = 0;
inline uint32_t millis() { return test_now; }
inline uint16_t crc16be(const uint8_t *data, size_t len, uint16_t crc) {
  for (size_t i=0; i<len; ++i) {
    crc ^= uint16_t(data[i]) << 8;
    for (int j=0;j<8;++j) crc = (crc & 0x8000) ? (crc << 1)^0x1021 : crc << 1;
  }
  return crc;
}
namespace setup_priority { constexpr float LATE = -100; }
class Component { public: virtual void setup() {} virtual void loop() {} virtual void dump_config() {} };
template<typename... Ts> class Action { public: virtual void play(const Ts &...) {} };
template<class T> class Parented { protected: T *parent_; };
}
