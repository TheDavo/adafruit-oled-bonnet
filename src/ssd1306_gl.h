#ifndef SSD1306_GL_H
#define SSD1306_GL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define ssd1306_gl_PAGE_HEIGHT 8

typedef struct ssd1306_fb_point {
  int x;
  int y;
} ssd1306_fb_point_t;

typedef struct ssd1306_fb {
  uint8_t height;
  uint8_t width;
  uint8_t *framebuf;
} ssd1306_fb_t;

typedef enum ssd1306_display_size {
  ssd1306_display_size_128_64,
  ssd1306_display_size_128_32,
} ssd1306_display_size_e;

/**
 * ssd1306_fb_new allocates memory for a framebuffer depending on
 * `display_size` and sets the pixels to `color`
 *
 */
ssd1306_fb_t *ssd1306_fb_new(ssd1306_display_size_e display_size, bool color);

/**
 * ssd1306_fb_free de-allocates the framebuffer inside of `self` and the
 * pointer itself
 *
 */
void ssd1306_fb_free(ssd1306_fb_t *self);

// primitive drawing functions
// these functions update the framebuffer, they do not actually send data
// to the ssd1306 device
//

/**
 * ssd1306_fb_clear_buffer calls memset on the buffer to set the entire buffer
 * to `color`
 *
 */
void ssd1306_fb_clear_buffer(ssd1306_fb_t *self, bool color);

/**
 * ssd1306_fb_bounds_check returns if both `x` and `y` are within the bounds
 * of the display's width and height
 *
 */
bool ssd1306_fb_bounds_check(ssd1306_fb_t self, int x, int y);

/**
 * ssd1306_fb_draw_pixel draws a pixel of color `color` to the location
 * (`x, y`) in the framebuffer
 *
 */
void ssd1306_fb_draw_pixel(ssd1306_fb_t *self, uint8_t x, uint8_t y,
                           bool color);

/**
 * ssd1306_fb_draw_line_carte draws a line from point (`x0, y0`) to (`x1, y1`)
 * with a color `color`
 *
 * The function name has the suffix "_carte" to show that this is for typical
 * Cartesian coordinates, and differs from `ssd1306_fb_draw_line_polar` which
 * takes in polar coorindates
 *
 */
void ssd1306_fb_draw_line_carte(ssd1306_fb_t *self, int x0, int y0, int x1,
                                int y1, bool color);

/**
 * ssd1306_fb_draw_line_polar draws a line using polar coordinate style inputs
 * from point (`x0, y0`) at a length `len` and angle (in degrees) `angle`
 * with a color `color`
 *
 */
void ssd1306_fb_draw_line_polar(ssd1306_fb_t *self, int x0, int y0,
                                uint32_t len, int angle, bool color);

/**
 * ssd1306_fb_draw_rect draws a rectangle, taking in inputs for the top left
 * corner `tl_x (top left x)`, `tl_y (top left y)`, its `width` and `height`,
 * draws it with the color `color`, and draws a filled
 * rectangle if `fill` is true
 *
 */
void ssd1306_fb_draw_rect(ssd1306_fb_t *self, int tl_x, int tl_y,
                          uint32_t length, uint32_t height, bool color,
                          bool fill);

/**
 * ssd1306_fb_draw_circle draws a circle with origin (`x0, y0`) and a radius
 * `radius` and color `color`, and draws a filled circle if `fill` is true
 *
 */
void ssd1306_fb_draw_circle(ssd1306_fb_t *self, int x0, int y0, int radius,
                            bool color, bool fill);

/**
 * ssd1306_fb_draw_arc draws an arc from what would be a circle with origin
 * (`x0, y0`), starting at the angle (both in degrees, 0-360) from
 * `arc_start_angle` to `arc_finish_angle` with color `color`
 *
 * If `fill` is true, the function draws a "slice" of a filled circle rather
 * than just an arc
 *
 * `resolution` sets the step size from `arc_start_angle` to `arc_finish_angle`
 * the lower `resolution` is, the finer the drawing
 *
 */
void ssd1306_fb_draw_arc(ssd1306_fb_t *self, int x0, int y0, uint8_t radius,
                         uint16_t arc_start_angle, uint16_t arc_finish_angle,
                         bool color, bool fill, uint8_t resolution);

void ssd1306_fb_draw_triangle(ssd1306_fb_t *self, ssd1306_fb_point_t v1,
                              ssd1306_fb_point_t v2, ssd1306_fb_point_t v3,
                              bool color);
#endif
