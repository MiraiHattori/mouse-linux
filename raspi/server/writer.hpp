#pragma once
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
    for (size_t i = 0; i < LOOP_NUM; i++) {
      int size;
      if (i != LOOP_NUM - 1) {
        size = write(m_mouse_fd_out, m_buf + i * BLOCKSIZE, BLOCKSIZE);
      } else {
        size = write(m_mouse_fd_out, m_buf + i * BLOCKSIZE,
                     BUFSIZE - BLOCKSIZE * i);
      }
      if (size < 0) {
        fprintf(stderr, "can't write\n");
        return;
      }
    }
  }
  void clearBuf() { std::fill(m_buf, m_buf + BUFSIZE, 0); }
  void clearRelBuf() {
    m_buf[X_L] = 0;
    m_buf[X_H] = 0;
    m_buf[Y_L] = 0;
    m_buf[Y_H] = 0;
    m_buf[WHEEL_L] = 0;
    m_buf[WHEEL_H] = 0;
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
  void xL(int x_l) { m_buf[X_L] = x_l; }
  void xH(int x_h) { m_buf[X_H] = x_h; }
  void yL(int y_l) { m_buf[Y_L] = y_l; }
  void yH(int y_h) { m_buf[Y_H] = y_h; }
  void wheelL(int wheel_l) { m_buf[WHEEL_L] = wheel_l; }
  void wheelH(int wheel_h) { m_buf[WHEEL_H] = wheel_h; }

private:
  static int m_mouse_fd_out;
  static uint8_t m_buf[BUFSIZE];
};