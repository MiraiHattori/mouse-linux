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
    int x = static_cast<int>(5.0f * sinf(t * 2.0f * 3.14f / 1.0f));
    int y = static_cast<int>(5.0f * cosf(t * 2.0f * 3.14f / 1.0f));
    int wheel = static_cast<int>(1.0f * cosf(t * 2.0f * 3.14f / 1.0f));
    writer.xL((x >> 0) & 0xff);
    writer.xH((x >> 8) & 0xff);
    writer.yL((y >> 0) & 0xff);
    writer.yH((y >> 8) & 0xff);
    writer.wheelL((wheel >> 0) & 0xff);
    writer.wheelH((wheel >> 8) & 0xff);
    writer.mywrite();
    usleep(15000);
    t += 0.015;
  }
  writer.finalize();
}
