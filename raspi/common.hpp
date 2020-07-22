#pragma once

#include <cstdint>

/* unused
#pragma pack(1)
// 5 byte
struct CommData {
  uint8_t buttons = 0; // left, right, middle, button3, button4, ..., button7
  // x_h: upper 8 bits, x_l: lower 8 bits
  // (x_h >> 8 + x_l) > 0 ? (x_h >> 8 + x_l) : 65536 + (x_h >> 8 + x_l)
  uint8_t x_l = 0;
  uint8_t x_h = 0;
  // y_h: upper 8 bits, y_l: lower 8 bits
  uint8_t y_l = 0;
  uint8_t y_h = 0;
  uint8_t wheel_l = 0;
  uint8_t wheel_h = 0;
};
#pragma pack()
*/

constexpr std::size_t BUFSIZE = 7;
constexpr std::size_t BLOCKSIZE = 4; // bytes transffered for one write
constexpr std::size_t LOOP_NUM =
    (BUFSIZE % BLOCKSIZE == 0)
        ? (BUFSIZE / BLOCKSIZE)
        : (BUFSIZE / BLOCKSIZE + 1); // bytes transffered for one write
constexpr std::size_t BUTTON = 0;
constexpr std::size_t X_L = 1;
constexpr std::size_t X_H = 2;
constexpr std::size_t Y_L = 3;
constexpr std::size_t Y_H = 4;
constexpr std::size_t WHEEL_L = 5;
constexpr std::size_t WHEEL_H = 6;
