#pragma once

#include "common.hpp"
#include <boost/asio.hpp>
#include <iostream>

namespace asio = boost::asio;
using asio::ip::tcp;

class Communicator {
public:
  explicit Communicator() = default;
  virtual void connect() = 0;
  virtual void disconnect() = 0;
  virtual uint8_t *read(size_t bufsize) = 0;

protected:
  uint8_t m_buf[BUFSIZE];
};

class WifiCommunicator : public Communicator {
public:
  explicit WifiCommunicator()
      : Communicator(), m_socket(tcp::socket(m_io_service)) {}
  void connect() override {
    tcp::acceptor acc(m_io_service, tcp::endpoint(tcp::v4(), 31400));
    acc.accept(m_socket);
  }
  void disconnect() override {}
  uint8_t *read(size_t bufsize) override {
    asio::streambuf receive_buffer;
    boost::system::error_code ec;
    if (asio::read(m_socket, receive_buffer, asio::transfer_exactly(bufsize),
                   ec) == 0) {
      std::cerr << "connection lost: " << ec.message() << std::endl;
      std::fill(m_buf, m_buf + bufsize, 0);
      return m_buf;
    }
    if (ec && ec != asio::error::eof) {
      std::cerr << "receive failed: " << ec.message() << std::endl;
      std::fill(m_buf, m_buf + bufsize, 0);
      return m_buf;
    }

    const uint8_t *data =
        asio::buffer_cast<const uint8_t *>(receive_buffer.data());
    std::copy(data, data + bufsize, m_buf);
    return m_buf;
  }

private:
  asio::io_service m_io_service;
  tcp::socket m_socket;
};
