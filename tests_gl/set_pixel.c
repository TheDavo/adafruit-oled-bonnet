#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"
#include "../src/bonnet.h"

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

  ssd1306_fb_draw_pixel(my_hat.ssd.framebuf, 50, 0, true);
  ssd1306_fb_draw_pixel(my_hat.ssd.framebuf, 51, 1, true);
  ssd1306_fb_draw_pixel(my_hat.ssd.framebuf, 52, 2, true);
  ssd1306_fb_draw_pixel(my_hat.ssd.framebuf, 53, 3, true);
  ssd1306_fb_draw_pixel(my_hat.ssd.framebuf, 54, 4, true);
  int bytes_written = ssd1306_write_framebuffer_all(my_hat.ssd);
  printf("[set_pixel] bytes written to framebuffer: %d\n", bytes_written);

  sleep(1);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);

}
