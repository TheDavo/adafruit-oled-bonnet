#include "progress_bar.h"
#include "../ssd1306_gl.h"

void uic_progress_bar_draw(ssd1306_fb_t *fb, void *_attr) {

  uic_progress_bar_attr_t *attr = (uic_progress_bar_attr_t *)_attr;

  // draw outside border
  if (attr->border) {
    ssd1306_fb_draw_rect(fb, attr->origin.x, attr->origin.y, attr->width,
                         attr->height, true, false);
  }

  uint8_t pad = attr->border ? attr->padding : 0;
  int prog_bar_x = attr->origin.x + pad;
  int prog_bar_y = attr->origin.y + pad;
  uint8_t prog_bar_width = attr->width - (2 * pad);
  uint8_t prog_bar_height = attr->height - (2 * pad);
  uint8_t progress = (int)(((double)attr->progress / 100) * prog_bar_width);

  ssd1306_fb_draw_rect(fb, prog_bar_x, prog_bar_y, progress, prog_bar_height,
                       true, true);
}
