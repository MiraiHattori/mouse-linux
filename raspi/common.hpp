#pragma once

#include <cstdint>

#pragma pack(1)
// 5 byte
struct CommData {
  uint8_t buttons = 0; // left, right, middle, button3, button4, ..., button7
  uint8_t x = 0;       // (x > 0 ? x : 256 + x)
  uint8_t y = 0;       // (y > 0 ? y : 256 + y)
  uint8_t wheel = 0;   // (wheel > 0 ? wheel : 256 + wheel)
};
#pragma pack()

constexpr std::size_t BUFSIZE = 4;
constexpr std::size_t BUTTON = 0;
constexpr std::size_t X = 1;
constexpr std::size_t Y = 2;
constexpr std::size_t WHEEL = 3;
