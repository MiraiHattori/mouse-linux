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
      std::this_thread::sleep_for(std::chrono::seconds(1));
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

class UartCommunicator : public Communicator {
public:
  explicit UartCommunicator()
      : Communicator(),
        m_serial(asio::serial_port(m_io_service, "/dev/ttyAMA0")) {}
  void connect() override {
    m_serial.set_option(asio::serial_port_base::baud_rate(115200));
    m_serial.set_option(asio::serial_port_base::character_size(8));
    m_serial.set_option(asio::serial_port_base::flow_control(
        asio::serial_port_base::flow_control::none));
    m_serial.set_option(
        asio::serial_port_base::parity(asio::serial_port_base::parity::none));
    m_serial.set_option(asio::serial_port_base::stop_bits(
        asio::serial_port_base::stop_bits::one));
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  void disconnect() override { m_serial.close(); }
  uint8_t *read(size_t bufsize) override {
    boost::system::error_code ec;
    size_t rest = bufsize;
    while (rest > 0) {
      size_t read_size = m_serial.read_some(asio::buffer(m_buf, bufsize), ec);
      rest -= read_size;
      for (size_t i = 0; i < read_size; i++) {
        std::cout << +m_buf[i] << " ";
      }
      std::cout << std::endl;
    }
    if (rest != 0) {
      std::cerr << "read failed: read_size " << bufsize - rest << " bytes"
                << std::endl;
    }
    if (ec && ec != asio::error::eof) {
      std::cerr << "read failed: " << ec.message() << std::endl;
      std::fill(m_buf, m_buf + bufsize, 0);
      return m_buf;
    }
    return m_buf;
  }

private:
  asio::io_service m_io_service;
  asio::serial_port m_serial;
};
