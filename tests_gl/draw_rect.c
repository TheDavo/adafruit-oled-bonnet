

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
  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);

  int width = my_hat.ssd.framebuf->width;
  int height = my_hat.ssd.framebuf->height;

  for (int x = 0; x < width; x += 8) {
    ssd1306_fb_draw_rect(my_hat.ssd.framebuf, 0, 0, x, x / 2, true, false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }
  for (int x = 0; x < width; x += 8) {
    ssd1306_fb_draw_rect(my_hat.ssd.framebuf, width - x, 0, x, x / 2, true,
                         false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, true);

  for (int x = 0; x < width; x += 8) {
    ssd1306_fb_draw_rect(my_hat.ssd.framebuf, 0, 0, x, x / 2, false, false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }
  for (int x = 0; x < width; x += 8) {
    ssd1306_fb_draw_rect(my_hat.ssd.framebuf, width - x - 1, 0, x, x / 2, false,
                         false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);
  for (int r = 0; r < height; r += 4) {
    ssd1306_fb_draw_rect(my_hat.ssd.framebuf, width / 2 - (r) - 1,
                         height / 2 - r - 1, 2 * r, 2 * r, true, false);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  sleep(3);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
}
