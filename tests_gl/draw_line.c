
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

// ---------------------------------------------
  printf("drawing lines from corners\n");
  for (int i = 0; i < 64; i += 8) {
    ssd1306_fb_draw_line_carte(my_hat.ssd.framebuf, 0, 0, 127, i, true);
  }
  ssd1306_write_framebuffer_all(my_hat.ssd);

  for (int i = 0; i < 64; i += 2) {
    ssd1306_fb_draw_line_carte(my_hat.ssd.framebuf, 127, 63, 0, i, true);
  }
  ssd1306_write_framebuffer_all(my_hat.ssd);
  sleep(1);
  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);

// ---------------------------------------------
  printf("drawing horizontal lines\n");
  for (int i = 0; i < height; i += 2) {
    ssd1306_fb_draw_line_carte(my_hat.ssd.framebuf, 0, i, width, i, true);
  }
  ssd1306_write_framebuffer_all(my_hat.ssd);
  sleep(1);
  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);

// ---------------------------------------------
  printf("drawing vertical lines\n");
  for (int i = 0; i < my_hat.ssd.framebuf->width; i += 8) {
    ssd1306_fb_draw_line_carte(my_hat.ssd.framebuf, i, height, i, 0, true);
  }
  ssd1306_write_framebuffer_all(my_hat.ssd);
  sleep(1);
  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);

// ---------------------------------------------
  printf("drawing lines from bottom center outward\n");
  int x0, y0;
  x0 = 128 / 2;
  y0 = 63;

  // double rad;
  for (double deg = 0; deg <= 180; deg += 10) {
    ssd1306_fb_draw_line_polar(my_hat.ssd.framebuf, x0, y0, 100, deg, true);
  }
  ssd1306_write_framebuffer_all(my_hat.ssd);

  sleep(1);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
}
