#include "mouse_reader.hpp"

int main() {
  MouseReader mouse_reader{};
  mouse_reader.initialize();
  while (true) {
    if (mouse_reader.myread()) {
      uint8_t *buf = mouse_reader.buf();
      for (size_t i = 0; i < BUFSIZE; i++) {
        printf("%d", buf[i]);
        if (i != BUFSIZE - 1) {
          printf(" ");
        }
      }
      printf("\n");
      mouse_reader.clearRelBuf();
    }
  }
  return 0;
}
