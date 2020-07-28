#include <boost/asio.hpp>
#include <iostream>

namespace asio = boost::asio;
using asio::ip::tcp;

class Communicator {
public:
  explicit Communicator() = default;
  virtual void connect() = 0;
  virtual void write(const uint8_t *buf, size_t bufsize) = 0;
  virtual void disconnect() = 0;
};

class WifiCommunicator : public Communicator {
public:
  explicit WifiCommunicator()
      : Communicator(), m_socket(tcp::socket(m_io_service)) {}
  void connect() override {
    tcp::resolver resolver(m_io_service);
    tcp::resolver::query query("raspberrypi0.local", "31400");
    tcp::resolver::iterator iterator;
  ON_RESOLVE:
    try {
      iterator = resolver.resolve(query);
    } catch (const boost::exception & /* e */) {
      goto ON_RESOLVE;
    }

  ON_CONNECT:
    try {
      m_socket = tcp::socket(m_io_service);
      m_socket.connect(iterator->endpoint());
    } catch (const boost::exception & /* e */) {
      goto ON_CONNECT;
    }
  }

  void write(const uint8_t *buf, size_t bufsize) override {
    boost::system::error_code ec;
    asio::write(m_socket, asio::buffer(buf, bufsize), ec);
    if (ec) {
      std::cerr << "send failed: " << ec.message() << std::endl;
    } else {
      for (size_t i = 0; i < bufsize; i++) {
        std::cout << buf[i] << " ";
      }
      std::cout << std::endl;
    }
  }
  void disconnect() override {}

private:
  asio::io_service m_io_service;
  tcp::socket m_socket;
};
