#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "../ssd1306_gl.h"
#include <stdint.h>

typedef struct uic_progress_bar_attr {
  ssd1306_fb_vec2_t origin;
  uint8_t width;
  uint8_t height;
  uint8_t progress; // 0 to 100
  uint8_t padding;  // padding of the progress bar to the border
  bool border;
} uic_progress_bar_attr_t;

void uic_progress_bar_draw(ssd1306_fb_t *fb, void *_attr);

#endif
