#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "type.h"

int g_mouse_fd_out;

void finalize() {
  if (ioctl(g_mouse_fd_out, UI_DEV_DESTROY) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  close(g_mouse_fd_out);
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
  struct uinput_setup uidev;
  memset(&uidev, 0, sizeof(uidev));
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor = 0x1234;
  uidev.id.product = 0x5678;
  strcpy(uidev.name, "myvirtmouse");
  // uidev.id.version = 1;
  if (ioctl(g_mouse_fd_out, UI_DEV_SETUP, &uidev) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  /*
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
  */
  if (ioctl(g_mouse_fd_out, UI_DEV_CREATE) < 0) {
    fprintf(stderr, "can't open mouse device\n");
    exit(0);
  }
  sleep(1);
}

int main() {

  initialize();
  /*
struct input_event {
    struct timeval time; // 入力のあった時間
    __u16 type; // EV_KEY, EV_REL, EV_ABS
    __u16 code; // KEY_SPACE, KEY_MUTE, BTN_LEFT, BTN_GEAR_DOWN, REL_X, ABS_X,
ABS_GAS
    __s32 value;
};
   */
  // for (int i = 0; i < 100; i++) {
  while (true) {
    struct input_event virt_mouse;
    // gettimeofday(&virt_mouse.time, NULL);
    virt_mouse.time.tv_sec = 0;
    virt_mouse.time.tv_usec = 0;
    virt_mouse.type = EV_REL;
    virt_mouse.code = REL_X;
    virt_mouse.value = 5;
    if (write(g_mouse_fd_out, &virt_mouse, sizeof(virt_mouse)) < 0) {
      printf("po\n");
      exit(0);
    }
    usleep(15000);
    virt_mouse.time.tv_sec = 0;
    virt_mouse.time.tv_usec = 0;
    virt_mouse.type = EV_SYN;
    virt_mouse.code = SYN_REPORT;
    virt_mouse.value = 0;
    if (write(g_mouse_fd_out, &virt_mouse, sizeof(virt_mouse)) < 0) {
      printf("po\n");
      exit(0);
    }
    usleep(15000);
  }
  finalize();
}
