#include "writer.hpp"
int main() {
  Writer writer{};
  writer.initialize();
  printf("click left\n");
  writer.clearBuf();
  writer.clickLeft(true);
  writer.mywrite();
  usleep(15000);
  // sleep 1 sec
  usleep(1000000);
  printf("release left\n");
  writer.clearBuf();
  writer.clickLeft(false);
  while (true) {
    writer.clearRelBuf();
    static float t = 0.0f;
    float x = 1.0f * sinf(t * 2.0f * 3.14f / 1.0f);
    float y = 1.0f * cosf(t * 2.0f * 3.14f / 1.0f);
    float wheel = 1.0f * cosf(t * 2.0f * 3.14f / 1.0f);
    writer.x(static_cast<int>(x));
    writer.y(static_cast<int>(y));
    writer.wheel(static_cast<int>(wheel));
    writer.mywrite();
    usleep(15000);
    t += 0.015;
  }
  writer.finalize();
}
