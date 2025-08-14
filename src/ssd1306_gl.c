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
  buffer_size /= ssd1306_gl_PAGE_HEIGHT;
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

  if (!ssd1306_fb_bounds_check(*self, x, y)) {
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

void ssd1306_fb_draw_rect(ssd1306_fb_t *self, int tl_x, int tl_y,
                          uint32_t length, uint32_t height, bool color,
                          bool fill) {
  int tr_x = tl_x + length;
  int tr_y = tl_y;
  int br_x = tl_x + length;
  int br_y = tl_y + height;
  int bl_x = tl_x;
  int bl_y = tl_y + height;

  // top horiz line
  ssd1306_fb_draw_line_carte(self, tl_x, tl_y, tr_x, tr_y, color);
  // left vert line
  ssd1306_fb_draw_line_carte(self, tl_x, tl_y, bl_x, bl_y, color);
  // right vert line
  ssd1306_fb_draw_line_carte(self, tr_x, tr_y, br_x, br_y, color);
  // bottom horiz line
  ssd1306_fb_draw_line_carte(self, bl_x, bl_y, br_x, br_y, color);

  if (fill) {
    for (int fill_y = tl_y; fill_y < bl_y; fill_y++) {
      ssd1306_fb_draw_line_carte(self, tl_x, fill_y, tr_x, fill_y,
                                 color);
    }
  }
}

//
// implementation of the draw_circle function is thanks to the explanation
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

void ssd1306_fb_draw_arc(ssd1306_fb_t *self, int x0, int y0, uint8_t radius,
                         uint16_t arc_start_angle, uint16_t arc_finish_angle,
                         bool color, bool fill, uint8_t resolution) {

  if (arc_start_angle == arc_finish_angle) {
    return;
  }

  if (resolution == 0) {
    return;
  }

  double rad;
  int x;
  int y;

  // always go from start angle to finish angle
  // to make the math easier and not have to do modulus complications
  // if the finish angle is less than the start, add a whole rotationm
  // sin and cos do not care!
  if (arc_finish_angle < arc_start_angle) {
    arc_start_angle += 360;
    int temp = arc_finish_angle;
    arc_finish_angle = arc_start_angle;
    arc_start_angle = temp;
  }

  for (int ang = arc_start_angle; ang <= arc_finish_angle; ang += resolution) {
    // convert angle to radians to use sin and cos functions
    rad = (double)(ang * M_PI / 180.0);
    x = x0 + cos(rad) * radius;
    y = y0 + sin(rad) * radius;
    ssd1306_fb_draw_pixel(self, x, y, color);
    if (fill) {
      ssd1306_fb_draw_line_polar(self, x0, y0, radius, ang, color);
    }
  }
}

int min(int a, int b) {
  if (a <= b) {
    return a;
  }

  return b;
}

int max(int a, int b) {
  if (a >= b) {
    return a;
  }

  return b;
}

//
// major thanks to Pikuma's video on triangle rasterization for helping
// understand and refresh ideas on cross products of vector and its meaning
// and how that can be used to determine if a point is "inside" the triangle
// https://www.youtube.com/watch?v=k5wtuKWmV48
//
int edge_cross(ssd1306_fb_vec2_t a, ssd1306_fb_vec2_t b, ssd1306_fb_vec2_t p) {
  ssd1306_fb_vec2_t ab = {
      .x = b.x - a.x,
      .y = b.y - a.y,
  };
  ssd1306_fb_vec2_t ap = {
      .x = p.x - a.x,
      .y = p.y - a.y,
  };
  return (ab.x * ap.y) - (ab.y * ap.x);
}

void ssd1306_fb_draw_triangle(ssd1306_fb_t *self, ssd1306_fb_vec2_t v0,
                              ssd1306_fb_vec2_t v1, ssd1306_fb_vec2_t v2,
                              bool color, bool fill) {
  ssd1306_fb_draw_line_carte(self, v0.x, v0.y, v1.x, v1.y, color);
  ssd1306_fb_draw_line_carte(self, v1.x, v1.y, v2.x, v2.y, color);
  ssd1306_fb_draw_line_carte(self, v0.x, v0.y, v2.x, v2.y, color);

  if (fill) {
    int x_min = min(min(v0.x, v1.x), v2.x);
    int y_min = min(min(v0.y, v1.y), v2.y);
    int x_max = max(max(v0.x, v1.x), v2.x);
    int y_max = max(max(v0.y, v1.y), v2.y);

    for (int y = y_min; y <= y_max; y++) {
      for (int x = x_min; x <= x_max; x++) {
        ssd1306_fb_vec2_t p = {x, y};
        int w0 = edge_cross(v0, v1, p);
        int w1 = edge_cross(v1, v2, p);
        int w2 = edge_cross(v2, v0, p);

        bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;
        if (is_inside) {
          ssd1306_fb_draw_pixel(self, x, y, color);
        }
      }
    }
  }
}

//
// the idea for this algorithm is to mix Bresenham's line and circle algorithms
// to draw the ellipse
//
// depending on the ellipse size, the x or y value will be changing faster
// than the other, and using that logic the previous algorithms can be modified
// to plot the points without having issues skiping points
//
// this is similar to how the line algorithms swap the x and y line inputs
// depending on the slope of the line (is it more horizontal or vertical?)
// 
void ssd1306_fb_draw_ellipse(ssd1306_fb_t *self, ssd1306_fb_vec2_t origin,
                             uint8_t x_radius, uint8_t y_radius, bool color,
                             bool fill) {
  int x, y, x_change, y_change, ellipse_error, two_a_square, two_b_square;
  int stopping_x, stopping_y;

  two_a_square = 2 * x_radius * x_radius;
  two_b_square = 2 * y_radius * y_radius;

  x = x_radius;
  y = 0;

  x_change = y_radius * y_radius * (1 - 2 * x_radius);
  y_change = x_radius * x_radius;
  ellipse_error = 0;
  stopping_x = two_b_square * x_radius;
  stopping_y = 0;

  while (stopping_x >= stopping_y) {
    ssd1306_fb_draw_pixel(self, origin.x + x, origin.y + y, color);
    ssd1306_fb_draw_pixel(self, origin.x - x, origin.y + y, color);
    ssd1306_fb_draw_pixel(self, origin.x - x, origin.y - y, color);
    ssd1306_fb_draw_pixel(self, origin.x + x, origin.y - y, color);
    if (fill) {
      ssd1306_fb_draw_line_carte(self, origin.x + x, origin.y + y, origin.x - x,
                                 origin.y + y, color);
      ssd1306_fb_draw_line_carte(self, origin.x + x, origin.y - y, origin.x - x,
                                 origin.y - y, color);
    }

    y++;
    stopping_y += two_a_square;
    ellipse_error += y_change;
    y_change += two_a_square;
    if ((2 * ellipse_error + x_change) > 0) {
      x--;
      stopping_x -= two_b_square;
      ellipse_error += x_change;
      x_change += two_b_square;
    }
  }

  // first set of points done, start the second set of points

  x = 0;
  y = y_radius;
  x_change = y_radius * y_radius;
  y_change = x_radius * x_radius * (1 - 2 * y_radius);
  ellipse_error = 0;
  stopping_x = 0;
  stopping_y = two_a_square * y_radius;

  while (stopping_x <= stopping_y) {
    ssd1306_fb_draw_pixel(self, origin.x + x, origin.y + y, color);
    ssd1306_fb_draw_pixel(self, origin.x - x, origin.y + y, color);
    ssd1306_fb_draw_pixel(self, origin.x - x, origin.y - y, color);
    ssd1306_fb_draw_pixel(self, origin.x + x, origin.y - y, color);
    if (fill) {
      ssd1306_fb_draw_line_carte(self, origin.x + x, origin.y + y, origin.x - x,
                                 origin.y + y, color);
      ssd1306_fb_draw_line_carte(self, origin.x + x, origin.y - y, origin.x - x,
                                 origin.y - y, color);
    }

    x++;
    stopping_x += two_b_square;
    ellipse_error += x_change;
    x_change += two_b_square;

    if ((2 * ellipse_error + y_change) > 0) {
      y--;
      stopping_y -= two_a_square;
      ellipse_error += y_change;
      y_change += two_a_square;
    }
  }
}
