#include "components.h"
#include "bar.h"
#include "segment16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uic_t *uic_progress_bar_new(uic_bar_attr_t *attr) {
  uic_t *progress_bar = malloc(sizeof(uic_t));
  if (NULL == progress_bar) {
    return NULL;
  }

  progress_bar->attr = attr;
  progress_bar->draw = uic_progress_bar_draw;

  return progress_bar;
}

uic_t *uic_segment16_new_from_char(char c, uic_segment16_attr_t *attr) {
  uic_t *segment = malloc(sizeof(uic_t));
  if (NULL == segment) {
    return NULL;
  }

  attr->segments = font_segment16[(int)c];
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
    uic_segment16_attr_set_segments(segments[i].attr, font_segment16[(int)c]);

    ((uic_segment16_attr_t *)(segments[i].attr))->origin.x =
        (o_originx + o_width / 2) * i;
    segments[i].draw = uic_segment16_draw;
  }

  return segments;
}
