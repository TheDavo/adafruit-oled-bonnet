
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

  uic_segment16_attr_t init_setting = {
      .origin = {.x = 10, .y = 30},
      .height = 16,
      .segments = 0,
      .dot = false,
      .color = true,
  };

  // test new_from_char
  uic_t *seg16 = uic_segment16_new_from_char('1', &init_setting);
  seg16->draw(b.ssd.framebuf, seg16->attr);
  ssd1306_write_framebuffer_all(b.ssd);
  sleep(1);
  ssd1306_fb_clear_buffer(b.ssd.framebuf, false);

  // test new_from_str
  init_setting.height = 9;
  char *str = "0123456789 10 12";
  int str_len = strlen(str);
  uic_t *seg_str = uic_segment16_new_from_str(str, str_len, &init_setting, 4);
  seg_str->draw(b.ssd.framebuf, seg_str->attr);
  ssd1306_write_framebuffer_all(b.ssd);

  ssd1306_fb_clear_buffer(b.ssd.framebuf, false);
  init_setting.origin.x = 1;
  init_setting.origin.y = 10;
  init_setting.height = 4;
  char *word_str = "THE QUICK BROWN FOX\nJUMPED OVER THE LAZY\nDOG";
  str_len = strlen(word_str);
  uic_t *seg_str_word =
      uic_segment16_new_from_str(word_str, str_len, &init_setting, 4);
  seg_str_word->draw(b.ssd.framebuf, seg_str_word->attr);
  ssd1306_write_framebuffer_all(b.ssd);

  sleep(3);

  // test new_from_int
  ssd1306_fb_clear_buffer(b.ssd.framebuf, false);
  init_setting.origin.x = 32;
  init_setting.height = 32;
  struct timespec start, stop;
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = -100; i <= 100; i += 3) {
    ssd1306_fb_clear_buffer(b.ssd.framebuf, false);
    uic_t *seg_int = uic_segment16_new_from_int(i, &init_setting, 4);
    seg_int->draw(b.ssd.framebuf, seg_int->attr);
    ssd1306_write_framebuffer_all(b.ssd);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);
  printf("fps: %f\n", ((double)200 / 3) / (stop.tv_sec - start.tv_sec));
  ssd1306_fb_clear_buffer(b.ssd.framebuf, false);

  sleep(1);
  free(seg16);
  bonnet_set_display_off(b);
  bonnet_close(&b);
}
