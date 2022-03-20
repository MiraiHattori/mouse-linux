#pragma once
#include <algorithm>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include "common.hpp"

class MouseReader {
public:
  explicit MouseReader() = default;
  static void initialize() {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
      printf("signal\n");
    }
    m_mouse_fd_in = open("/dev/input/event0", O_RDONLY);
    if (m_mouse_fd_in < 0) {
      fprintf(stderr, "can't open mouse device\n");
      exit(0);
    }
  }
  static void clearBuf() { std::fill(m_buf, m_buf + BUFSIZE, 0); }

  static void clearRelBuf() {
    m_buf[X_L] = 0;
    m_buf[X_H] = 0;
    m_buf[Y_L] = 0;
    m_buf[Y_H] = 0;
    m_buf[WHEEL_L] = 0;
    m_buf[WHEEL_H] = 0;
  }

  static uint8_t *buf() { return m_buf; }

  //  if true; ready to send
  static bool myread() {
    struct input_event mouse;
    if (read(m_mouse_fd_in, &mouse, sizeof(struct input_event)) < 0) {
      fprintf(stderr, "[MouseReader] can't read mouse event");
      return false;
    }
    if (mouse.type == EV_SYN) {
      return true;
    }
    if (mouse.type == EV_KEY) {
      if (mouse.code == BTN_LEFT) {
        if (mouse.value == 0) {
          m_buf[BUTTON] &= (0xff - (0b1 << 0));
        } else {
          m_buf[BUTTON] |= (0b1 << 0);
        }
      } else if (mouse.code == BTN_RIGHT) {
        if (mouse.value == 0) {
          m_buf[BUTTON] &= (0xff - (0b1 << 1));
        } else {
          m_buf[BUTTON] |= (0b1 << 1);
        }
      } else if (mouse.code == BTN_MIDDLE) {
        if (mouse.value == 0) {
          m_buf[BUTTON] &= (0xff - (0b1 << 2));
        } else {
          m_buf[BUTTON] |= (0b1 << 2);
        }
      } else if (mouse.code == BTN_SIDE) { // temae
        if (mouse.value == 0) {
          m_buf[BUTTON] &= (0xff - (0b1 << 3));
        } else {
          m_buf[BUTTON] |= (0b1 << 3);
        }
      } else if (mouse.code == BTN_EXTRA) { // oku
        if (mouse.value == 0) {
          m_buf[BUTTON] &= (0xff - (0b1 << 4));
        } else {
          m_buf[BUTTON] |= (0b1 << 4);
        }
      }
    } else if (mouse.type == EV_REL) {
      if (mouse.code == REL_WHEEL) {
        m_buf[WHEEL_L] = (mouse.value >> 0) & 0xff;
        m_buf[WHEEL_H] = (mouse.value >> 8) & 0xff;
      } else if (mouse.code == REL_X) {
        m_buf[X_L] = (mouse.value >> 0) & 0xff;
        m_buf[X_H] = (mouse.value >> 8) & 0xff;
      } else if (mouse.code == REL_Y) {
        m_buf[Y_L] = (mouse.value >> 0) & 0xff;
        m_buf[Y_H] = (mouse.value >> 8) & 0xff;
      }
    } else if (mouse.type == EV_MSC) {
      // left click code 0x90001 / 0d589825
      // right click code 0x90002 / 0d589826
      // middle click code 0x90003 / 0d589827
      // side click code 0x90004 / 0d589828
      // extra click code 0x90005 / 0d589829
      // unused
    }
    printf("[MouseReader] ");
    for (size_t i = 0; i < BUFSIZE; i++) {
      printf("%d ", m_buf[i]);
    }
    printf("\n");
    return false;
  }
  static void sigint_handler(int /* signo */) {
    printf("\nSIGINT\n");
    finalize();
    exit(0);
  }
  static void finalize() { close(m_mouse_fd_in); }

private:
  static int m_mouse_fd_in;
  static uint8_t m_buf[BUFSIZE];
};
