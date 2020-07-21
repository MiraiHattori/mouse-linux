#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "writer.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;

std::mutex mtx;
Writer writer{};
bool clicked = false;
bool end = false;

// read only for mutex object except for writer
static void modifyThread() {
  constexpr size_t MAX_NUM = 100;
  int modify_x[MAX_NUM] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  int modify_y[MAX_NUM] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  std::chrono::time_point<std::chrono::system_clock> start_t;
  constexpr double interval_ms = 30.0;
  bool last_clicked = false;
  int cnt = 0;
  while (not end) {
    bool c = clicked;
    if (not c) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<int>(interval_ms)));
      last_clicked = c;
      continue;
    }
    if (not last_clicked) {
      start_t = std::chrono::system_clock::now();
      cnt = 0;
    }
    {
      std::lock_guard<std::mutex> lock(mtx);
      writer.xL(modify_x[cnt]);
      writer.xH(0);
      writer.yL(modify_y[cnt]);
      writer.yH(0);
      writer.mywrite();
      writer.clearRelBuf();
    }
    cnt++;
    // stay in the last modif
    if (cnt == MAX_NUM) {
      cnt--;
    }
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::system_clock::now() - start_t)
                           .count();
    int sleep_ms = static_cast<int>(cnt * interval_ms - duration_ms);
    if (sleep_ms > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }
    last_clicked = c;
  }
}

int main() {
  asio::io_service io_service;

  tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), 31400));
  tcp::socket socket(io_service);

  writer.initialize();
  writer.clearBuf();
  std::thread modify_thread(modifyThread);
  modify_thread.detach();

  // 接続待機
  acc.accept(socket);

  while (true) {
    // メッセージ受信
    asio::streambuf receive_buffer;
    boost::system::error_code error;
    if (asio::read(socket, receive_buffer, asio::transfer_exactly(BUFSIZE),
                   error) == 0) {
      std::cout << "connection lost: " << error.message() << std::endl;
      break;
    }
    if (error && error != asio::error::eof) {
      std::cout << "receive failed: " << error.message() << std::endl;
      break;
    } else {
      const uint8_t *data =
          asio::buffer_cast<const uint8_t *>(receive_buffer.data());
      for (size_t i = 0; i < BUFSIZE; i++) {
        printf("%d ", data[i]);
      }
      int x_l = data[X_L];
      int x_h = data[X_H];
      int y_l = data[Y_L];
      int y_h = data[Y_H];
      int wheel_l = data[WHEEL_L];
      int wheel_h = data[WHEEL_H];
      uint8_t b = data[BUTTON];
      {
        std::lock_guard<std::mutex> lock(mtx);
        writer.clickLeft(((b & (0b1 << 0)) >> 0) == 1);
        writer.clickRight(((b & (0b1 << 1)) >> 1) == 1);
        writer.clickMiddle(((b & (0b1 << 2)) >> 2) == 1);
        writer.clickSide(((b & (0b1 << 3)) >> 3) == 1);
        writer.clickExtra(((b & (0b1 << 4)) >> 4) == 1);
        writer.xL(x_l);
        writer.xH(x_h);
        writer.yL(y_l);
        writer.yH(y_h);
        writer.wheelL(wheel_l);
        writer.wheelH(wheel_h);
        writer.mywrite();
        writer.clearRelBuf();
      }
      printf("\n");
    }
  }
  end = true;
  return 0;
}
