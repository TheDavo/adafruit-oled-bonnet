#include "./cursor.h"

void uic_cursor_draw(ssd1306_fb_t *fb, void *_attr) {

  // for now the cursor is going to be a simple crosshair style cursor
  // for simplicity, with 'c' denoting the center (not part of the render)
  //     |
  //     |
  //     |
  //
  //
  //
  // --- c ---
  //
  //
  //
  //     |
  //     |
  //     |

  uic_cursor_attr_t *attr = (uic_cursor_attr_t *)_attr;

  int len = 9;

  for (int i = 0; i < 3; i++) {
    // draw top vertical bar
    ssd1306_fb_draw_pixel(fb, attr->origin.x + len / 2, attr->origin.y + i,
                          true);
    // draw bot vertical bar
    ssd1306_fb_draw_pixel(fb, attr->origin.x + len / 2,
                          attr->origin.y + len - i - 1, true);

    // draw left horiz bar
    ssd1306_fb_draw_pixel(fb, attr->origin.x + i, attr->origin.y + len / 2,
                          true);
    // draw right horiz bar
    ssd1306_fb_draw_pixel(fb, attr->origin.x + len - i - 1,
                          attr->origin.y + len / 2, true);
  }
}

uic_t *uic_cursor_new(uic_cursor_attr_t *attr) {
  uic_t *cursor = malloc(sizeof(uic_t));
  if (NULL == cursor) {
    return NULL;
  }

  attr->pointing_to.x = attr->origin.x + (9 / 2);
  attr->pointing_to.y = attr->origin.y + (9 / 2);

  cursor->attr = attr;
  cursor->draw = uic_cursor_draw;

  return cursor;
}


void uic_cursor_position_update_relative(uic_t *cursor, int x_inc, int y_inc){
  uic_cursor_attr_t *attr = (uic_cursor_attr_t *)cursor->attr;
  attr->origin.x += x_inc;
  attr->origin.y += y_inc;
  attr->pointing_to.x = attr->origin.x + (9 / 2);
  attr->pointing_to.y = attr->origin.y + (9 / 2);
}
