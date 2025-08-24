#include "../src/bonnet.h"
#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"

// #include <bits/time.h>
// #include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  struct bonnet b;
  int init_ret = bonnet_struct_init(&b, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  bonnet_display_initialize(b);
  bonnet_action_clear_display(&b);

  char *text = "Hello Eli!";
  ssd1306_fb_vec2_t origin = {
    .x = 16,
    .y = 32,
  };

  char *text2 = "- Davo";
  ssd1306_fb_vec2_t o2 = {
    .x = 24,
    .y = 48,
  };

  ssd1306_fb_draw_8x8font_str(b.ssd.framebuf, origin, text, 11, true, false);
  ssd1306_fb_draw_8x8font_str(b.ssd.framebuf, o2, text2, 7, true, false);
  ssd1306_write_framebuffer_all(b.ssd);

  sleep(5);
  
  bonnet_set_display_off(b);
  bonnet_close(&b);
}
