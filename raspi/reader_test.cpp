#include "reader.hpp"

int main() {
  Reader reader{};
  reader.initialize();
  while (true) {
    if (reader.myread()) {
      uint8_t *buf = reader.buf();
      for (size_t i = 0; i < BUFSIZE; i++) {
        printf("%d", buf[i]);
        if (i != BUFSIZE - 1) {
          printf(" ");
        }
      }
      printf("\n");
      reader.clearRelBuf();
    }
  }
  return 0;
}
