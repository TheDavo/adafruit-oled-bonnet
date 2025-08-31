#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../ssd1306_gl.h"
#include "bar.h"
#include <stdint.h>

// uic -> UI component
typedef struct uic {
  void *attr;
  // use member attr variable
  void (*draw)(ssd1306_fb_t *, void *); 
} uic_t;

void uic_free(uic_t *component);

uic_t *uic_progress_bar_new(uic_bar_attr_t *attr);

#endif
