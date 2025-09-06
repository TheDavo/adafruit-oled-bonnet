#ifndef BAR_H
#define BAR_H

#include "../ssd1306_gl.h"
#include "./components.h"
#include <stdint.h>


typedef enum {
  PROGRESS_TYPE_PERCENTAGE, // e.g 50%
  PROGRESS_TYPE_FRACTION,   // e.g 50/100
  PROGRESS_TYPE_CURRENT,
} uic_bar_e_progress_type;

typedef struct uic_bar_attr {
  ssd1306_fb_vec2_t origin;
  uint8_t width;
  uint8_t height;
  uint32_t progress_current; // 0 to `progress_to`
  uint32_t progress_to;
  uint8_t padding;  // padding of the progress bar to the border

  // draw style
  bool border;
  bool display_progress;
  uic_bar_e_progress_type progress_display_type;
} uic_bar_attr_t;

void uic_progress_bar_draw(ssd1306_fb_t *fb, void *_attr);

uic_t *uic_progress_bar_new(uic_bar_attr_t *attr);

#endif
