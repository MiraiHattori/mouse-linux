#include "mouse_writer.hpp"
int main() {
  MouseWriter mouse_writer{};
  mouse_writer.initialize();
  printf("click left\n");
  mouse_writer.clearBuf();
  mouse_writer.clickLeft(true);
  mouse_writer.mywrite();
  usleep(15000);
  // sleep 1 sec
  usleep(1000000);
  printf("release left\n");
  mouse_writer.clearBuf();
  mouse_writer.clickLeft(false);
  while (true) {
    mouse_writer.clearRelBuf();
    static float t = 0.0f;
    int x = static_cast<int>(5.0f * sinf(t * 2.0f * 3.14f / 1.0f));
    int y = static_cast<int>(5.0f * cosf(t * 2.0f * 3.14f / 1.0f));
    int wheel = static_cast<int>(1.0f * cosf(t * 2.0f * 3.14f / 1.0f));
    mouse_writer.xL((x >> 0) & 0xff);
    mouse_writer.xH((x >> 8) & 0xff);
    mouse_writer.yL((y >> 0) & 0xff);
    mouse_writer.yH((y >> 8) & 0xff);
    mouse_writer.wheelL((wheel >> 0) & 0xff);
    mouse_writer.wheelH((wheel >> 8) & 0xff);
    mouse_writer.mywrite();
    usleep(15000);
    t += 0.015;
  }
  mouse_writer.finalize();
}
