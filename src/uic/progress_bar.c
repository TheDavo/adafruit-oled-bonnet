#include "progress_bar.h"
#include "../ssd1306_gl.h"
#include <stdio.h>

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
  uint8_t progress_inv =
      (int)((1 - (double)attr->progress / 100) * prog_bar_width);

  ssd1306_fb_draw_rect(fb, prog_bar_x, prog_bar_y, progress, prog_bar_height,
                       true, true);

  if (attr->display_progress) {
    // when inverting the text, not drawing the empty progress bar would
    // cause jittering/glitching in the text rendering
    ssd1306_fb_draw_rect(fb, progress + prog_bar_x, prog_bar_y,
                         progress_inv, prog_bar_height, false, true);
    ssd1306_fb_vec2_t text_origin = {
        // attr->origin is the origin of the rectangle
        .x = attr->origin.x + attr->width / 2 - 16, // offset by 2 font width
        .y = attr->origin.y + attr->height / 2 - 4, // offset by font height/2
    };
    // account for null
    char progress_txt[5];
    sprintf(progress_txt, "%3d%%", attr->progress);
    ssd1306_fb_draw_8x8font_str(fb, text_origin, progress_txt, 4, true, true);
  }
}
