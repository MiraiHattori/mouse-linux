#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "type.h"

int g_mouse_fd_in;

void finalize() { close(g_mouse_fd_in); }

static void signal_handler(int signo) {
  printf("\nSIGINT\n");
  finalize();
  exit(0);
}

void initialize() {
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    printf("signal\n");
  }

  g_mouse_fd_in = open("/dev/input/event9", O_RDONLY);
  if (g_mouse_fd_in < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
}

int main() {

  initialize();
  struct input_event mouse;
  /*
struct input_event {
    struct timeval time; // 入力のあった時間
    __u16 type; // EV_KEY, EV_REL, EV_ABS
    __u16 code; // KEY_SPACE, KEY_MUTE, BTN_LEFT, BTN_GEAR_DOWN, REL_X, ABS_X,
ABS_GAS
    __s32 value;
};
   */
  for (;;) {
    int bytes = read(g_mouse_fd_in, &mouse, sizeof(struct input_event));
    if (bytes >= 0) {
      printEventType(mouse.type, mouse.code, mouse.value, true);
      if (mouse.type == EV_REL) {
        if (mouse.code == REL_WHEEL) {
          printf("REL_WHEEL %d", mouse.value);
        } else if (mouse.code == REL_X) {
          printf("REL_X %d", mouse.value);
        } else if (mouse.code == REL_Y) {
          printf("REL_Y %d", mouse.value);
        } else {
          printf("%hu %d", mouse.code, mouse.value);
        }
      }
      /*
      if (mouse.type == EV_KEY && mouse.code == BTN_LEFT) {
        printf("break!\n");
        break;
      }
      */
      printf("\n");
    }
  }
  finalize();
}
