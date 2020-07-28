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
  void disconnect() override {}

  void write(const uint8_t *buf, size_t bufsize) override {
    boost::system::error_code ec;
    asio::write(m_socket, asio::buffer(buf, bufsize), ec);
    if (ec) {
      std::cerr << "send failed: " << ec.message() << std::endl;
    } else {
      for (size_t i = 0; i < bufsize; i++) {
        std::cout << +buf[i] << " ";
      }
      std::cout << std::endl;
    }
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
  void write(const uint8_t *buf, size_t bufsize) override {
    boost::system::error_code ec;
    size_t sent_size = m_serial.write_some(asio::buffer(buf, bufsize), ec);
    if (sent_size != bufsize) {
      std::cerr << "write failed: sent " << sent_size << " bytes" << std::endl;
    }
    if (ec) {
      std::cerr << "send failed: " << ec.message() << std::endl;
    } else {
      std::cout << "[UartCommunicator] ";
      for (size_t i = 0; i < bufsize; i++) {
        std::cout << +buf[i] << " ";
      }
      std::cout << std::endl;
    }
  }

private:
  asio::io_service m_io_service;
  asio::serial_port m_serial;
};
