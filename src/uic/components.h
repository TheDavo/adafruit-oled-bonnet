#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../ssd1306_gl.h"
#include "bar.h"
#include "segment16.h"
#include <stdint.h>

// uic -> UI component
typedef struct uic {
  void *attr;
  // use member attr variable
  void (*draw)(ssd1306_fb_t *, void *);
} uic_t;

void uic_free(uic_t *component);

uic_t *uic_progress_bar_new(uic_bar_attr_t *attr);

uic_t *uic_segment16_new(uic_segment16_attr_t *attr);
uic_t *uic_segment16_new_from_char(char c, uic_segment16_attr_t *attr);
uic_t *uic_segment16_new_from_str(char *str, int str_len,
                                   uic_segment16_attr_t *init_setting);

#endif
