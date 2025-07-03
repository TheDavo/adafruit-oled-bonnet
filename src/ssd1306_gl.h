#ifndef SSD1306_GL_H
#define SSD1306_GL_H

#include <stdint.h>
#include <sys/types.h>

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
ssd1306_fb_t ssd1306_fb_free(ssd1306_fb_t *self);

// primitive drawing functions
// these functions update the framebuffer, they do not actually send data
// to the ssd1306 device

void ssd1306_fb_draw_pixel(ssd1306_fb_t *self, uint8_t x, uint8_t y,
                           bool color);

void ssd1306_fb_draw_line_carte(ssd1306_fb_t *self, int x0, int y0, int x1,
                               int y1, bool color);

void ssd1306_fb_draw_line_polar(ssd1306_fb_t *self, int x0, int y0, int len,
                                int angle, bool color);

void ssd1306_fb_draw_rect(ssd1306_fb_t *self, int tl, int tr, int br, int bl,
                          bool color);

void ssd1306_fb_draw_circle(ssd1306_fb_t *self, int x0, int y0, int radius,
                            bool color);

void ssd1306_fb_draw_arc(ssd1306_fb_t *self, int x0, int y0, uint8_t radius,
                         int arc_start_degree, int arc_finish_degree,
                         bool color);

#endif
