#ifndef UIC_CURSOR_H
#define UIC_CURSOR_H
#include "../ssd1306_gl.h"
#include "./components.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct uic_cursor_attr {
  // origin is still the top-left corner of the bounding box of the cursor
  // not where the cursor is pointing to
  ssd1306_fb_vec2_t origin;
  // pointing_to is the "center" of the cursor
  ssd1306_fb_vec2_t pointing_to;
} uic_cursor_attr_t;

void uic_cursor_draw(ssd1306_fb_t *fb, void *_attr);

uic_t *uic_cursor_new(uic_cursor_attr_t *attr);

void uic_cursor_position_update_relative(uic_t *cursor, int x_inc, int y_inc);

#endif
