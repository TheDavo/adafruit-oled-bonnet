#include "components.h"
#include "progress_bar.h"
#include <stdlib.h>

uic_t *uic_progress_bar_new(uic_progress_bar_attr_t *attr) {
  uic_t *progress_bar = malloc(sizeof(uic_t));
  if (NULL == progress_bar) {
    return NULL;
  }

  progress_bar->attr = attr;
  progress_bar->draw = uic_progress_bar_draw;

  return progress_bar;
}
