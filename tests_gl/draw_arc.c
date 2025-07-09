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

  for (int i = 0; i <= 180; i += 10) {
    ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, false);
    ssd1306_fb_draw_arc(my_hat.ssd.framebuf, width / 2, height / 2, 20, i,
                        360 - i, true, false, 1);

    ssd1306_fb_draw_arc(my_hat.ssd.framebuf, 0, 0, 20, i, 360 - i, true,
                        false, 1);
    ssd1306_fb_draw_arc(my_hat.ssd.framebuf, width, 0, 20, i, 360 - i, true,
                        false, 1);
    ssd1306_fb_draw_arc(my_hat.ssd.framebuf, 0, height, 20, i, 360 - i, true,
                        false, 1);
    ssd1306_fb_draw_arc(my_hat.ssd.framebuf, width, height, 20, i, 360 - i,
                        true, false, 1);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  for (int i = 0; i <= 180; i += 10) {
    ssd1306_fb_clear_buffer(my_hat.ssd.framebuf, true);
    ssd1306_fb_draw_arc(my_hat.ssd.framebuf, width / 2, height / 2, 20, 180 - i,
                        180 + i, false, true, 1);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }

  sleep(1);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
}
