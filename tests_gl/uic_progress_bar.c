#include "../src/bonnet.h"
#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"
#include "../src/uic/components.h"
#include "../src/uic/bar.h"

#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
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

  uic_bar_attr_t attr = {
      .origin =
          {
              .x = 10,
              .y = 10,
          },
      .width = 100,
      .height = 10,
      .progress_current = 50,
      .progress_to = 100,
      .border = true,
      .padding = 3,
      .display_progress = false,
      .progress_display_type = PROGRESS_TYPE_CURRENT,
  };

  uic_bar_attr_t attr2 = {
      .origin =
          {
              .x = 8,
              .y = 32,
          },
      .width = 100,
      .height = 15,
      .progress_current = 1024,
      .progress_to = 2048,
      .border = true,
      .padding = 2,
      .display_progress = true,
      .progress_display_type = PROGRESS_TYPE_FRACTION,
  };

  struct timespec start, stop;
  uic_t *prog_bar = uic_progress_bar_new(&attr);
  uic_t *prog_bar2 = uic_progress_bar_new(&attr2);
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int progress = 0; progress <= 100; progress += 2) {
    ((uic_bar_attr_t *)prog_bar->attr)->progress_current = progress;
    ((uic_bar_attr_t *)prog_bar2->attr)->progress_current =
        progress * 20;
    prog_bar->draw(my_hat.ssd.framebuf, prog_bar->attr);
    prog_bar2->draw(my_hat.ssd.framebuf, prog_bar2->attr);
    // printf("framebuffer bytes written: %d\n",
    // ssd1306_write_framebuffer_all_new(&my_hat.ssd));
    // ssd1306_write_framebuffer_all_new(&my_hat.ssd);
    ssd1306_write_framebuffer_all(my_hat.ssd);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);
  printf("fps: %f\n", 50.0 / (stop.tv_sec - start.tv_sec));

  sleep(3);
  free(prog_bar);
  free(prog_bar2);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
}
