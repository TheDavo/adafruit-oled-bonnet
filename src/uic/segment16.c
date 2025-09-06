#include "segment16.h"
#include "../ssd1306_gl.h"
#include <stdlib.h>
#include <string.h>

// Reference of the segments
//     00   01
//   ----.----
//   0203040506
//    | \ | / |
//     07   08
//   ----.----
//   0910111213
//    | / | \ |
//   14     15
//   ----.----

//
// array of functions to dynamically call the appropriate draw function
//
void (*uic_segment16_draw_fns[16])(ssd1306_fb_t *, void *) = {
    uic_segment16__draw_0,  uic_segment16__draw_1,  uic_segment16__draw_2,
    uic_segment16__draw_3,  uic_segment16__draw_4,  uic_segment16__draw_5,
    uic_segment16__draw_6,  uic_segment16__draw_7,  uic_segment16__draw_8,
    uic_segment16__draw_9,  uic_segment16__draw_10, uic_segment16__draw_11,
    uic_segment16__draw_12, uic_segment16__draw_13, uic_segment16__draw_14,
    uic_segment16__draw_15,
};

void uic_segment16_draw(ssd1306_fb_t *fb, void *_attr) {
  // The segments attribute needs to be looped over, and if the bit is set,
  // then the segment can be drawn
  //

  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;

  for (int shift = 0; shift < 16; shift++) {
    if (attr->segments & (0x1 << shift)) {
      uic_segment16_draw_fns[shift](fb, _attr);
    }
  }
}

void uic_segment16__draw_0(ssd1306_fb_t *fb, void *_attr) {
  // top left horizontal bar
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_1(ssd1306_fb_t *fb, void *_attr) {
  // top right horizontal bar
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width / 2;
  int y0 = attr->origin.y;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

// Reference of the segments
//     00   01
//   ----.----
//   0203040506
//    | \ | / |
//     07   08
//   ----.----
//   0910111213
//    | / | \ |
//   14     15
//   ----.----

void uic_segment16__draw_2(ssd1306_fb_t *fb, void *_attr) {
  // top left vertical bar
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y;
  int x1 = x0;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}
void uic_segment16__draw_3(ssd1306_fb_t *fb, void *_attr) {
  // top left diagonal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y;
  int x1 = x0 + width / 2;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_4(ssd1306_fb_t *fb, void *_attr) {
  // top middle vertical line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width / 2;
  int y0 = attr->origin.y;
  int x1 = x0;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_5(ssd1306_fb_t *fb, void *_attr) {
  // top right diagonal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width;
  int y0 = attr->origin.y;
  int x1 = attr->origin.x + width / 2;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_6(ssd1306_fb_t *fb, void *_attr) {
  // top right vertical line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width;
  int y0 = attr->origin.y;
  int x1 = x0;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_7(ssd1306_fb_t *fb, void *_attr) {
  // middle left horizontal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y + attr->height / 2;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

// Reference of the segments
//     00   01
//   ----.----
//   0203040506
//    | \ | / |
//     07   08
//   ----.----
//   0910111213
//    | / | \ |
//   14     15
//   ----.----
//

void uic_segment16__draw_8(ssd1306_fb_t *fb, void *_attr) {
  // middle right horizontal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width / 2;
  int y0 = attr->origin.y + attr->height / 2;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_9(ssd1306_fb_t *fb, void *_attr) {
  // bottom left vertical line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  // unused in this func
  // uint8_t width = attr->height / 2;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y + attr->height / 2;
  int x1 = x0;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_10(ssd1306_fb_t *fb, void *_attr) {
  // bottom left diagonal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y + attr->height;
  int x1 = x0 + width / 2;
  int y1 = y0 - attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_11(ssd1306_fb_t *fb, void *_attr) {
  // bottom middle vertical line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width / 2;
  int y0 = attr->origin.y + attr->height / 2;
  int x1 = x0;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_12(ssd1306_fb_t *fb, void *_attr) {
  // bottom right diagonal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width / 2;
  int y0 = attr->origin.y + attr->height / 2;
  int x1 = x0 + width / 2;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_13(ssd1306_fb_t *fb, void *_attr) {
  // bottom right vertical line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width;
  int y0 = attr->origin.y + attr->height / 2;
  int x1 = x0;
  int y1 = y0 + attr->height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_14(ssd1306_fb_t *fb, void *_attr) {
  // bottom left horizontal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x;
  int y0 = attr->origin.y + attr->height;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

void uic_segment16__draw_15(ssd1306_fb_t *fb, void *_attr) {
  // bottom right horizontal line
  uic_segment16_attr_t *attr = (uic_segment16_attr_t *)_attr;
  uint8_t width = attr->height / 2;

  int x0 = attr->origin.x + width / 2;
  int y0 = attr->origin.y + attr->height;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr->color);
}

//
// setter/getter funcs
//

void uic_segment16_attr_set_segments(uic_segment16_attr_t *attr,
                                     uint16_t segments) {
  attr->segments = segments;
}

//
// new* funcs
//

uic_t *uic_segment16_new_from_char(char c, uic_segment16_attr_t *attr) {
  uic_t *segment = malloc(sizeof(uic_t));
  if (NULL == segment) {
    return NULL;
  }

  attr->segments = uic_segment16_font[(int)c];
  segment->attr = attr;

  segment->draw = uic_segment16_draw;
  return segment;
}

uic_t *uic_segment16_new_from_str(char *str, int str_len,
                                  uic_segment16_attr_t *init_setting) {
  uic_t *segments = malloc(sizeof(uic_t) * str_len);
  if (NULL == segments) {
    return NULL;
  }

  int o_originx = init_setting->origin.x;
  // int o_originy = init_setting->origin.y;
  int o_height = init_setting->height;
  int o_width = o_height / 2;

  for (int i = 0; i < str_len; i++) {
    uic_segment16_attr_t *seg_attr = malloc(sizeof(uic_segment16_attr_t));
    if (NULL == seg_attr) {
      free(segments);
      return NULL;
    }

    memcpy(seg_attr, init_setting, sizeof(uic_segment16_attr_t));
    segments[i].attr = seg_attr;

    char c = str[i];
    uic_segment16_attr_set_segments(segments[i].attr,
                                    uic_segment16_font[(int)c]);

    ((uic_segment16_attr_t *)(segments[i].attr))->origin.x =
        (o_originx + o_width / 2) * i;
    segments[i].draw = uic_segment16_draw;
  }

  return segments;
}
