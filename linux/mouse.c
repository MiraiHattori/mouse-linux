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
int g_mouse_fd_out;

void finalize() {
  if (ioctl(g_mouse_fd_out, UI_DEV_DESTROY) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  close(g_mouse_fd_out);
  close(g_mouse_fd_in);
}

static void signal_handler(int signo) {
  printf("\nSIGINT\n");
  finalize();
  exit(0);
}

void initialize() {
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    printf("signal\n");
  }

  g_mouse_fd_in = open("/dev/input/event5", O_RDONLY);
  if (g_mouse_fd_in < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  g_mouse_fd_out = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  if (g_mouse_fd_out < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  if (ioctl(g_mouse_fd_out, UI_SET_EVBIT, EV_KEY) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  if (ioctl(g_mouse_fd_out, UI_SET_KEYBIT, BTN_LEFT) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  if (ioctl(g_mouse_fd_out, UI_SET_EVBIT, EV_REL) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  if (ioctl(g_mouse_fd_out, UI_SET_RELBIT, REL_X) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  if (ioctl(g_mouse_fd_out, UI_SET_RELBIT, REL_Y) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  struct uinput_user_dev uidev = {0};
  snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "myvirtmouse");
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor = 0x7777;
  uidev.id.product = 0x7777;
  uidev.id.version = 1;
  if (write(g_mouse_fd_out, &uidev, sizeof(uidev)) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  uidev.absmin[REL_X] = 0;
  uidev.absmax[REL_X] = 1024;
  uidev.absfuzz[REL_X] = 0;
  uidev.absflat[REL_X] = 0;
  uidev.absmin[REL_Y] = 0;
  uidev.absmax[REL_Y] = 1024;
  uidev.absfuzz[REL_Y] = 0;
  uidev.absflat[REL_Y] = 0;
  if (write(g_mouse_fd_out, &uidev, sizeof(uidev)) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  if (ioctl(g_mouse_fd_out, UI_DEV_CREATE) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
}

int main() {

  initialize();
  struct input_event mouse;
  struct input_event virt_mouse;
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
          gettimeofday(&virt_mouse.time, NULL);
          virt_mouse.type = mouse.type;
          virt_mouse.code = mouse.code;
          virt_mouse.value = -mouse.value;
          if (write(g_mouse_fd_out, &virt_mouse, sizeof(virt_mouse)) < 0) {
            printf("po\n");
            exit(0);
          }
          gettimeofday(&virt_mouse.time, NULL);
          virt_mouse.type = EV_SYN;
          virt_mouse.code = 0;
          virt_mouse.value = 0;
          if (write(g_mouse_fd_out, &virt_mouse, sizeof(virt_mouse)) < 0) {
            printf("po\n");
            exit(0);
          }
        } else if (mouse.code == REL_Y) {
          // printf("REL_Y %d", mouse.value);
        } else {
          printf("%hu %d", mouse.code, mouse.value);
        }
      }
      if (mouse.type == EV_KEY && mouse.code == BTN_LEFT) {
        printf("break!\n");
        break;
      }
    }
    printf("\n");
  }
  finalize();
}
