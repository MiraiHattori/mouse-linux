#include <boost/asio.hpp>
#include <iostream>

#include "reader.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;

int main() {
  asio::io_service io_service;
  tcp::resolver resolver(io_service);
  tcp::resolver::query query("raspberrypi0.local", "31400");
  tcp::resolver::iterator iterator = resolver.resolve(query);

  // 接続
  tcp::socket socket(io_service);
  socket.connect(iterator->endpoint());

  Reader reader{};
  reader.initialize();

  // メッセージ送信
  while (true) {
    if (reader.myread()) {
      const uint8_t *buf = reader.buf();
      boost::system::error_code error;
      asio::write(socket, asio::buffer(buf, BUFSIZE), error);
      reader.clearRelBuf();
      if (error) {
        std::cout << "send failed: " << error.message() << std::endl;
        break;
      } else {
        for (size_t i = 0; i < BUFSIZE; i++) {
          printf("%d ", buf[i]);
        }
        printf("\n");
      }
    }
  }
}
