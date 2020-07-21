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
      int x = (data[X] > 0 ? data[X] : 256 - data[X]);
      int y = (data[Y] > 0 ? data[Y] : 256 - data[Y]);
      int wheel = (data[WHEEL] > 0 ? data[WHEEL] : 256 - data[WHEEL]);
      uint8_t b = data[BUTTON];
      writer.clickLeft(((b & (0b1 << 0)) >> 0) == 1);
      writer.clickRight(((b & (0b1 << 1)) >> 1) == 1);
      writer.clickMiddle(((b & (0b1 << 2)) >> 2) == 1);
      writer.clickSide(((b & (0b1 << 3)) >> 3) == 1);
      writer.clickExtra(((b & (0b1 << 4)) >> 4) == 1);
      writer.x(x);
      writer.y(y);
      writer.wheel(wheel);
      writer.mywrite();
      writer.clearRelBuf();
      printf("\n");
    }
  }
}
