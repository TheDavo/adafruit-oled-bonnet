
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
  // int height = my_hat.ssd.framebuf->height;

  ssd1306_fb_point_t v1 = {
    .x = 10,
    .y = 10,
  };

  ssd1306_fb_point_t v2 = {
    .x = width - 10,
    .y = 10,
  };

  ssd1306_fb_point_t v3 = {
    .x = 45,
    .y = 60,
  };

  for (int v3x = 10; v3x <= width - 10; v3x += 4) {
    ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);
    v3.x = v3x;
    ssd1306_fb_draw_triangle(my_hat.ssd.framebuf, v1, v2, v3, true);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  sleep(1);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
}
