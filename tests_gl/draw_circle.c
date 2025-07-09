#include "../src/bonnet.h"
#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  struct bonnet my_hat;
  int init_ret = bonnet_struct_init(&my_hat, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  bonnet_display_initialize(my_hat);
  bonnet_action_clear_display(&my_hat);

  int width = my_hat.ssd.framebuf->width;
  int height = my_hat.ssd.framebuf->height;

  // ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, true);
  for (int i = 0; i < 20; i++) {
    ssd1306_fb_draw_circle(my_hat.ssd.framebuf, width / 2, height / 2,
                           10 + 2 * i, true, false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }
  for (int i = 19; i >= 0; i--) {
    ssd1306_fb_draw_circle(my_hat.ssd.framebuf, width / 2, height / 2,
                           10 + 2 * i, false, false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  for (int i = 0; i < 10; i++) {
    ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, true);
    ssd1306_fb_draw_circle(my_hat.ssd.framebuf, width / 2, height / 2,
                           10 + 2 * i, false, true);

    ssd1306_fb_draw_circle(my_hat.ssd.framebuf, 0, height / 2, 10 + i, false,
                           true);

    ssd1306_fb_draw_circle(my_hat.ssd.framebuf, width - 1, height / 2, 10 + i,
                           false, true);
    usleep(5000);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);
  // draw a circle of circles
  int radius = 20;
  int mini_radius = 5;
  double rad;
  int x, y;
  for (int offset = 0; offset <= 360; offset += 10) {
    ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);

    for (int ang = 0; ang < 360; ang += 45) {
      rad = (double)((ang + offset) * M_PI / 180.0);
      x = ((double)width / 2) + cos(rad) * radius;
      y = ((double)height / 2) + sin(rad) * radius;
      ssd1306_fb_draw_circle(my_hat.ssd.framebuf, x, y, mini_radius, true,
                             false);
    }
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }
  sleep(3);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
}
