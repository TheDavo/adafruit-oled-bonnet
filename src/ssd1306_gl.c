#include "ssd1306_gl.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//
// pseudo-private functions (written here but not in header file)
//

void ssd1306_gl__draw_line_horiz(ssd1306_fb_t *self, int x0, int y0, int x1,
                                 int y1, bool color);
void ssd1306_gl__draw_line_verti(ssd1306_fb_t *self, int x0, int y0, int x1,
                                 int y1, bool color);

ssd1306_fb_t *ssd1306_fb_new(ssd1306_display_size_e display_size, bool color) {

  int buffer_size = 0;
  uint8_t height, width;
  switch (display_size) {
  case ssd1306_display_size_128_32:
    buffer_size = 128 * 32;
    height = 32;
    width = 128;
    break;
  case ssd1306_display_size_128_64:
    buffer_size = 128 * 64;
    height = 64;
    width = 128;
    break;
  default:
    // should never reach here!
    return NULL;
  }

  // allocate space for the framebuffer
  uint8_t *framebuf = malloc(buffer_size * sizeof(uint8_t));
  if (NULL == framebuf) {
    return NULL;
  }

  // set the framebuffer to an initial color depending on `color`
  memset(framebuf, color, buffer_size);

  ssd1306_fb_t *fb = malloc(sizeof(ssd1306_fb_t));
  if (NULL == fb) {
    free(framebuf);
    return NULL;
  }

  fb->framebuf = framebuf;
  fb->height = height;
  fb->width = width;

  return fb;
}

void ssd1306_fb_free(ssd1306_fb_t *self) {
  if (self) {
    if (self->framebuf) {
      free(self->framebuf);
    }
    free(self);
  }
}

//
// Drawing functions
//

void ssd1306_fb_clear_buffer(ssd1306_fb_t *self, bool color) {
  int buf_size = (self->height * self->width) / ssd1306_gl_PAGE_HEIGHT;
  int set_val = 0;
  if (color) {
    set_val = 0xFF;
  }
  memset(self->framebuf, set_val, buf_size);
}

bool ssd1306_fb_bounds_check(ssd1306_fb_t self, int x, int y) {
  bool is_x_okay = (x >= 0) && (x < self.width);
  bool is_y_okay = (y >= 0) && (y < self.height);

  return is_x_okay && is_y_okay;
}

void ssd1306_fb_draw_pixel(ssd1306_fb_t *self, uint8_t x, uint8_t y,
                           bool color) {
  if (!self) {
    return;
  }

  if (!self->framebuf) {
    return;
  }

  if (!ssd1306_fb_bounds_check(*self, x, y)){
    return;
  }

  int pixel_buffer_location = (y / ssd1306_gl_PAGE_HEIGHT) * self->width + x;
  uint8_t pixel_value = self->framebuf[pixel_buffer_location];
  if (color) {
    pixel_value |= (1 << (y % ssd1306_gl_PAGE_HEIGHT));
  } else {
    pixel_value &= ~(1 << (y % ssd1306_gl_PAGE_HEIGHT));
  }

  self->framebuf[pixel_buffer_location] = pixel_value;
}

//
// the following draw_line_horiz and draw_line_verti functions use
// the tried and true Bresenham's algorithm to draw a line
//
// additional help and clarification of how this algorithm works
// can be found at https://youtu.be/CceepU1vIKo?si=6QrNO0oBxqxvvLTo
// from NoBS Code
//
void ssd1306_gl__draw_line_horiz(ssd1306_fb_t *self, int x0, int y0, int x1,
                                 int y1, bool color) {

  int dx = x1 - x0;
  int dy = y1 - y0;

  int dir = (dy < 0) ? -1 : 1;

  dy *= dir;

  if (0 != dx) {
    int y = y0;
    int decision = 2 * dy - dx;
    for (int i = 0; i < dx + 1; i++) {
      if (ssd1306_fb_bounds_check(*self, x0 + i, y)) {
        ssd1306_fb_draw_pixel(self, x0 + i, y, color);
      }

      if (decision >= 0) {
        y += dir;
        decision = decision - 2 * dx;
      }
      decision = decision + 2 * dy;
    }
  }
}

void ssd1306_gl__draw_line_verti(ssd1306_fb_t *self, int x0, int y0, int x1,
                                 int y1, bool color) {

  int dx = x1 - x0;
  int dy = y1 - y0;

  int dir = (dx < 0) ? -1 : 1;

  dx *= dir;

  if (0 != dy) {
    int x = x0;
    int decision = 2 * dx - dy;
    for (int i = 0; i < dy + 1; i++) {
      if (ssd1306_fb_bounds_check(*self, x, y0 + i)) {
        ssd1306_fb_draw_pixel(self, x, y0 + i, color);
      }

      if (decision >= 0) {
        x += dir;
        decision = decision - 2 * dy;
      }
      decision = decision + 2 * dx;
    }
  }
}

void ssd1306_fb_draw_line_carte(ssd1306_fb_t *self, int x0, int y0, int x1,
                                int y1, bool color) {

  if (abs(x1 - x0) > abs(y1 - y0)) { // horizontal line
    if (x0 > x1) {
      ssd1306_gl__draw_line_horiz(self, x1, y1, x0, y0, color);
    } else {
      ssd1306_gl__draw_line_horiz(self, x0, y0, x1, y1, color);
    }
  } else {
    if (y0 > y1) { // vertical line
      ssd1306_gl__draw_line_verti(self, x1, y1, x0, y0, color);
    } else {
      ssd1306_gl__draw_line_verti(self, x0, y0, x1, y1, color);
    }
  }
}

void ssd1306_fb_draw_line_polar(ssd1306_fb_t *self, int x0, int y0,
                                uint32_t len, int angle, bool color) {
  // convert to Cartesian coordinates, then call draw_line_carte

  double rad;
  rad = angle * M_PI / 180;
  int x = x0 + (int)(cos(rad) * len);
  int y = y0 - (int)(sin(rad) * len);
  ssd1306_fb_draw_line_carte(self, x0, y0, x, y, color);
}

//
// implemenation of the draw_circle function is thanks to the explanation
// video from NoBS code:
// https://youtu.be/hpiILbMkF9w?si=7yMTLhzgZev_Oumi
//
void ssd1306_fb_draw_circle(ssd1306_fb_t *self, int x0, int y0, int radius,
                            bool color, bool fill) {
  int x = 0;
  int y = -radius;

  // midpoint decision
  int mpd = -radius;

  while (x < -y) {
    if (mpd > 0) {
      y += 1;
      mpd += 2 * (x + y) + 1;
    } else {
      mpd += 2 * x + 1;
    }

    ssd1306_fb_draw_pixel(self, x0 + x, y0 + y, color);
    ssd1306_fb_draw_pixel(self, x0 - x, y0 + y, color);
    ssd1306_fb_draw_pixel(self, x0 + x, y0 - y, color);
    ssd1306_fb_draw_pixel(self, x0 - x, y0 - y, color);
    ssd1306_fb_draw_pixel(self, x0 + y, y0 + x, color);
    ssd1306_fb_draw_pixel(self, x0 - y, y0 + x, color);
    ssd1306_fb_draw_pixel(self, x0 + y, y0 - x, color);
    ssd1306_fb_draw_pixel(self, x0 - y, y0 - x, color);

    if (fill) {
      ssd1306_fb_draw_line_carte(self, x0 + x, y0 + y, x0 - x, y0 + y, color);
      ssd1306_fb_draw_line_carte(self, x0 + x, y0 - y, x0 - x, y0 - y, color);
      ssd1306_fb_draw_line_carte(self, x0 + y, y0 + x, x0 - y, y0 + x, color);
      ssd1306_fb_draw_line_carte(self, x0 + y, y0 - x, x0 - y, y0 - x, color);
    }

    x += 1;
  }
}
