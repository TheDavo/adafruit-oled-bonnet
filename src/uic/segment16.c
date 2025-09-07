#include "segment16.h"
#include "../ssd1306_gl.h"
#include <stdio.h>
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
void (*uic_segment16_draw_fns[16])(ssd1306_fb_t *, uic_segment16_attr_t) = {
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
  int len = attr->len;
  uic_segment16_attr_t *end = attr + len;

  while (attr < end) {
    for (int shift = 0; shift < 16; shift++) {
      if (attr->segments & (0x1 << shift)) {
        // uic_segment16_draw_fns[shift](fb, &attr[i]);
        uic_segment16_draw_fns[shift](fb, *attr);
      }
    }
    attr++;
  }

  // for (int i = 0; i < len; i++) {
  //   // printf("[uic_segment16_draw] i: %d, len: %d\n", i, attr->len);
  //   uic_segment16_attr_t cur_attr = attr[i];
  //   printf("attr[%d] len: %u\n%p\n", i, cur_attr.len, (void *)&cur_attr);
  //   for (int shift = 0; shift < 16; shift++) {
  //     if (cur_attr.segments & (0x1 << shift)) {
  //       // uic_segment16_draw_fns[shift](fb, &attr[i]);
  //       uic_segment16_draw_fns[shift](fb, &cur_attr);
  //     }
  //   }
  // }
}

void uic_segment16__draw_0(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top left horizontal bar
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x;
  int y0 = attr.origin.y;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_1(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top right horizontal bar
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width / 2;
  int y0 = attr.origin.y;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
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

void uic_segment16__draw_2(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top left vertical bar

  int x0 = attr.origin.x;
  int y0 = attr.origin.y;
  int x1 = x0;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}
void uic_segment16__draw_3(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top left diagonal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x;
  int y0 = attr.origin.y;
  int x1 = x0 + width / 2;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_4(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top middle vertical line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width / 2;
  int y0 = attr.origin.y;
  int x1 = x0;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_5(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top right diagonal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width;
  int y0 = attr.origin.y;
  int x1 = attr.origin.x + width / 2;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_6(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // top right vertical line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width;
  int y0 = attr.origin.y;
  int x1 = x0;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_7(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // middle left horizontal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x;
  int y0 = attr.origin.y + attr.height / 2;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
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

void uic_segment16__draw_8(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // middle right horizontal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width / 2;
  int y0 = attr.origin.y + attr.height / 2;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_9(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom left vertical line
  // unused in this func
  // uint8_t width = attr.height / 2;

  int x0 = attr.origin.x;
  int y0 = attr.origin.y + attr.height / 2;
  int x1 = x0;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_10(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom left diagonal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x;
  int y0 = attr.origin.y + attr.height;
  int x1 = x0 + width / 2;
  int y1 = y0 - attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_11(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom middle vertical line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width / 2;
  int y0 = attr.origin.y + attr.height / 2;
  int x1 = x0;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_12(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom right diagonal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width / 2;
  int y0 = attr.origin.y + attr.height / 2;
  int x1 = x0 + width / 2;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_13(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom right vertical line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width;
  int y0 = attr.origin.y + attr.height / 2;
  int x1 = x0;
  int y1 = y0 + attr.height / 2;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_14(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom left horizontal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x;
  int y0 = attr.origin.y + attr.height;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
}

void uic_segment16__draw_15(ssd1306_fb_t *fb, uic_segment16_attr_t attr) {
  // bottom right horizontal line
  uint8_t width = attr.height / 2;

  int x0 = attr.origin.x + width / 2;
  int y0 = attr.origin.y + attr.height;
  int x1 = x0 + width / 2;
  int y1 = y0;
  ssd1306_fb_draw_line_carte(fb, x0, y0, x1, y1, attr.color);
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
  attr->len = 1;
  segment->attr = attr;

  segment->draw = uic_segment16_draw;
  return segment;
}

uic_t *uic_segment16_new_from_int(int num, uic_segment16_attr_t *init_setting,
                                  int spacing) {
  char buff[20];
  snprintf(buff, 20, "%d", num);

  int len = strlen(buff);
  uic_t *segments = malloc(sizeof(uic_t) * len);
  if (NULL == segments) {
    return NULL;
  }

  uic_segment16_attr_t *seg_attr = malloc(sizeof(uic_segment16_attr_t) * len);
  if (NULL == seg_attr) {
    free(segments);
    return NULL;
  }

  // save the original values to use for spacing
  int o_originx = init_setting->origin.x;
  int o_height = init_setting->height;
  int o_width = o_height / 2;

  for (int i = 0; i < len; i++) {
    memcpy(&seg_attr[i], init_setting, sizeof(uic_segment16_attr_t));
    seg_attr[i].len = 1;
    if (0 == i) {
      seg_attr[i].len = len;
    }

    char c = buff[i];
    seg_attr[i].segments = uic_segment16_font[(int)c];
    seg_attr[i].origin.x = (o_originx) + (o_width + spacing) * i;
    segments[i].draw = uic_segment16_draw;
    segments[i].attr = &seg_attr[i];
  }

  return segments;
}

uic_t *uic_segment16_new_from_str(char *str, int str_len,
                                  uic_segment16_attr_t *init_setting,
                                  int spacing) {
  uic_t *segments = malloc(sizeof(uic_t) * str_len);
  if (NULL == segments) {
    return NULL;
  }

  uic_segment16_attr_t *seg_attr =
      malloc(sizeof(uic_segment16_attr_t) * str_len);
  if (NULL == seg_attr) {
    free(segments);
    return NULL;
  }

  int o_originx = init_setting->origin.x;
  int o_originy = init_setting->origin.y;
  int o_height = init_setting->height;
  int o_width = o_height / 2;

  // help account for newlines in strings
  int newline_cnt = 0;
  int new_x = o_originx;
  int new_x_i = 0;

  for (int i = 0; i < str_len; i++) {

    memcpy(&seg_attr[i], init_setting, sizeof(uic_segment16_attr_t));
    seg_attr[i].len = 1;
    if (0 == i) {
      seg_attr[i].len = str_len;
    }

    char c = str[i];
    if ('\n' == c) {
      newline_cnt++;
      new_x = o_originx;
      new_x_i = 0;
    } else {
      new_x = o_originx + (o_width + spacing) * new_x_i;
      new_x_i++;
    }
    seg_attr[i].segments = uic_segment16_font[(int)c];
    // seg_attr[i].origin.x = (o_originx) + (o_width + spacing) * i;
    seg_attr[i].origin.x = new_x;
    seg_attr[i].origin.y = (o_originy) + (o_height + spacing) * newline_cnt;
    segments[i].attr = &seg_attr[i];
    segments[i].draw = uic_segment16_draw;
  }

  return segments;
}
