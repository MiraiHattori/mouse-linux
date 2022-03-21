#include <boost/asio.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "communicator.hpp"
#include "mouse_writer.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;

std::mutex mtx;
MouseWriter mouse_writer{};
bool clicked = false;
bool end = false;

// read only for mutex object except for mouse_writer
static void modifyThread() {
  constexpr size_t MAX_NUM = (27 - 1) * 5;
  int modify_xy[MAX_NUM][2];
  {
    std::ifstream ifs("/home/pi/mouse-linux/raspi/server/pattern.txt");
    if (not ifs) {
      std::cout << "could not open file" << std::endl;
    }
    int line = 0;
    std::string str;
    while (std::getline(ifs, str)) {
      std::istringstream iss(str);
      std::string tmp;
      std::getline(iss, tmp, ',');
      int x = std::stoi(tmp);
      modify_xy[line][0] = x;
      std::getline(iss, tmp, ',');
      int y = std::stoi(tmp);
      modify_xy[line][1] = y;
      line++;
    }
  }

  std::chrono::time_point<std::chrono::system_clock> start_t;
  constexpr double interval_us = 1000000.0 / 18.0 / 5.0;
  bool last_clicked = false;
  int cnt = 0;
  while (not end) {
    bool c = clicked;
    if (not c) {
      std::this_thread::sleep_for(
          std::chrono::microseconds(static_cast<int>(interval_us)));
      last_clicked = c;
      continue;
    }
    if (not last_clicked) {
      start_t = std::chrono::system_clock::now();
      cnt = 0;
    }
    if (cnt == MAX_NUM - 1) {
      std::this_thread::sleep_for(
          std::chrono::microseconds(static_cast<int>(interval_us)));
      last_clicked = c;
      continue;
    }
    {
      std::lock_guard<std::mutex> lock(mtx);
      int x = modify_xy[cnt][0];
      int x_l = (x > 0) ? (x >> 0 & 0xff) : (((65536 + x) >> 0) & 0xff);
      int x_h = (x > 0) ? (x >> 8 & 0xff) : (((65536 + x) >> 8) & 0xff);
      int y = modify_xy[cnt][1];
      int y_l = (y > 0) ? (y >> 0 & 0xff) : (((65536 + y) >> 0) & 0xff);
      int y_h = (y > 0) ? (y >> 8 & 0xff) : (((65536 + y) >> 8) & 0xff);
      mouse_writer.xL(x_l);
      mouse_writer.xH(x_h);
      mouse_writer.yL(y_l);
      mouse_writer.yH(y_h);
      mouse_writer.mywrite();
      mouse_writer.clearRelBuf();
    }
    cnt++;
    // stay in the last modif
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(
                           std::chrono::system_clock::now() - start_t)
                           .count();
    int sleep_us = static_cast<int>(cnt * interval_us - duration_us);
    if (sleep_us > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(sleep_us));
    }
    last_clicked = c;
  }
}

int main() {
  std::shared_ptr<Communicator> communicator =
      // std::make_shared<WifiCommunicator>();
      std::make_shared<UartCommunicator>();
  communicator->connect();
  mouse_writer.initialize();
  mouse_writer.clearBuf();
  std::thread modify_thread(modifyThread);
  modify_thread.detach();

  while (true) {
    uint8_t *data = communicator->read(BUFSIZE);
    int x_l = data[X_L];
    int x_h = data[X_H];
    int y_l = data[Y_L];
    int y_h = data[Y_H];
    int wheel_l = data[WHEEL_L];
    int wheel_h = data[WHEEL_H];
    uint8_t b = data[BUTTON];
    {
      std::lock_guard<std::mutex> lock(mtx);
      clicked = (((b & (0b1 << 0)) >> 0) == 1);
      mouse_writer.clickLeft(((b & (0b1 << 0)) >> 0) == 1);
      mouse_writer.clickRight(((b & (0b1 << 1)) >> 1) == 1);
      mouse_writer.clickMiddle(((b & (0b1 << 2)) >> 2) == 1);
      mouse_writer.clickSide(((b & (0b1 << 3)) >> 3) == 1);
      mouse_writer.clickExtra(((b & (0b1 << 4)) >> 4) == 1);
      mouse_writer.xL(x_l);
      mouse_writer.xH(x_h);
      mouse_writer.yL(y_l);
      mouse_writer.yH(y_h);
      mouse_writer.wheelL(wheel_l);
      mouse_writer.wheelH(wheel_h);
      mouse_writer.mywrite();
      mouse_writer.clearRelBuf();
    }
  }
  // TODO (future731) デストラクタでjoinする
  end = true;
  return 0;
}
