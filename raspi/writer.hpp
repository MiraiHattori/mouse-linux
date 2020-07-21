#pragma once
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <math.h>
#include <string>
#include <unistd.h>

#include "common.hpp"

class Writer {
public:
  explicit Writer() = default;
  static void initialize() {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
      printf("signal\n");
    }

    m_mouse_fd_out = open("/dev/hidg0", O_RDWR);
    if (m_mouse_fd_out < 0) {
      fprintf(stderr, "can't open mouse device\n");
      exit(0);
    }
  }
  static void sigint_handler(int /* signo */) {
    printf("\nSIGINT\n");
    finalize();
    exit(0);
  }
  static void finalize() { close(m_mouse_fd_out); }

  void mywrite() {
    if (write(m_mouse_fd_out, m_buf, sizeof(m_buf)) < 0) {
      fprintf(stderr, "can't write\n");
    }
  }
  void clearBuf() { std::fill(m_buf, m_buf + BUFSIZE, 0); }
  void clearRelBuf() {
    m_buf[X] = 0;
    m_buf[Y] = 0;
    m_buf[WHEEL] = 0;
  }
  // if click == false, that means releasing the button
  void clickLeft(const bool click) {
    if (click) {
      m_buf[BUTTON] |= (0b1 << 0);
    } else {
      m_buf[BUTTON] &= (0xff - (0b1 << 0));
    }
  }
  void clickRight(const bool click) {
    if (click) {
      m_buf[BUTTON] |= (0b1 << 1);
    } else {
      m_buf[BUTTON] &= (0xff - (0b1 << 1));
    }
  }
  void clickMiddle(const bool click) {
    if (click) {
      m_buf[BUTTON] |= (0b1 << 2);
    } else {
      m_buf[BUTTON] &= (0xff - (0b1 << 2));
    }
  }
  void clickSide(const bool click) {
    if (click) {
      m_buf[BUTTON] |= (0b1 << 3);
    } else {
      m_buf[BUTTON] &= (0xff - (0b1 << 3));
    }
  }
  void clickExtra(const bool click) {
    if (click) {
      m_buf[BUTTON] |= (0b1 << 4);
    } else {
      m_buf[BUTTON] &= (0xff - (0b1 << 4));
    }
  }
  void x(int x) { m_buf[X] = (x > 0 ? x : 256 + x); }
  void y(int y) { m_buf[Y] = (y > 0 ? y : 256 + y); }
  void wheel(int x) { m_buf[WHEEL] = (x > 0 ? x : 256 + x); }

private:
  static int m_mouse_fd_out;
  static uint8_t m_buf[BUFSIZE];
};
