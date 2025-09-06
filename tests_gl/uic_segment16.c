
#include "../src/bonnet.h"
#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"
#include "../src/uic/components.h"
#include "../src/uic/segment16.h"

#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
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

  uic_segment16_attr_t attr = {
      .origin = {.x = 10, .y = 30},
      .height = 16,
      .segments = 0,
      .dot = false,
      .color = true,
  };

  uic_t *seg16 = uic_segment16_new_from_char('1', &attr);
  seg16->draw(b.ssd.framebuf, seg16->attr);
  ssd1306_write_framebuffer_all(b.ssd);
  sleep(2);
  ssd1306_fb_clear_buffer(b.ssd.framebuf, false);

  // array of uic_t
  attr.height = 9;
  char *str = "0123456789101010101";
  int str_len = strlen(str);
  uic_t *seg_str = uic_segment16_new_from_str(str, str_len, &attr);
  for (int i = 0; i < str_len; i++) {
    seg_str[i].draw(b.ssd.framebuf, seg_str[i].attr);
  }
  ssd1306_write_framebuffer_all(b.ssd);

  ssd1306_fb_clear_buffer(b.ssd.framebuf, false);

  // print '0' to '9'
  for (int ch = 48; ch < 58; ch++) {
    uic_t *seg_ch = uic_segment16_new_from_char((char)ch, &attr);
    seg_ch->draw(b.ssd.framebuf, &attr);
    ssd1306_write_framebuffer_all(b.ssd);
    // sleep(1);
    usleep(10 * 1000); // 10 msec
    ssd1306_fb_clear_buffer(b.ssd.framebuf, false);
    
  }
  
  sleep(3);
  free(seg16);
  bonnet_set_display_off(b);
  bonnet_close(&b);
}
