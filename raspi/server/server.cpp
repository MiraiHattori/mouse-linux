#include <boost/asio.hpp>
#include <iostream>

#include "writer.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;

int main() {
  asio::io_service io_service;

  tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), 31400));
  tcp::socket socket(io_service);

  Writer writer{};
  writer.initialize();
  writer.clearBuf();

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
      printf("\n");
    }
  }
}
