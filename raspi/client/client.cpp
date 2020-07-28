#include <boost/asio.hpp>
#include <iostream>
#include <memory>

#include "communicator.hpp"
#include "mouse_reader.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;

int main() {
  std::shared_ptr<Communicator> communicator =
      std::make_shared<WifiCommunicator>();

  MouseReader mouse_reader{};
  mouse_reader.initialize();

  // メッセージ送信
  while (true) {
    if (mouse_reader.myread()) {
      const uint8_t *buf = mouse_reader.buf();
      communicator->write(buf, BUFSIZE);
      mouse_reader.clearRelBuf();
    }
  }
}
