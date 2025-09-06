#include "segment16.h"
#include "../ssd1306_gl.h"

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

void uic_segment16_attr_set_segments(uic_segment16_attr_t *attr,
                                    uint16_t segments) {
  attr->segments = segments;
}
