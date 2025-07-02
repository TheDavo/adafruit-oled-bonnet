#include "davo_gl.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

davo_gl_t *davo_gl_new_nullable() {
  davo_gl_t *d_gl = malloc(sizeof(davo_gl_t));
  return d_gl;
}

void davo_gl_free(struct davo_gl *davo_gl) {
  if (NULL != davo_gl) {
    switch (davo_gl->framebuf.pixel_type) {
    case davo_gl_e_pixel_type_monochrome:
      free(davo_gl->framebuf.buffer.mono_buffer);
      break;
    case davo_gl_e_pixel_type_monochrome8:
      free(davo_gl->framebuf.buffer.mono8_buffer);
      break;
    }
    free(davo_gl);
    davo_gl = NULL;
  }
}

int davo_gl_framebuffer_new(struct davo_gl *davo_gl,
                            enum davo_gl_pixel_type_e pixel_type, int width,
                            int height) {
  davo_gl->framebuf.pixel_type = pixel_type;
  davo_gl->width = width;
  davo_gl->height = height;

  int bufsize = width * height;

  switch (davo_gl->framebuf.pixel_type) {
  case davo_gl_e_pixel_type_monochrome:
    davo_gl->framebuf.buffer.mono_buffer =
        malloc(bufsize * sizeof(davo_gl_pixel_monochrome_t));

    if (NULL == davo_gl->framebuf.buffer.mono_buffer) {
      return (-1);
    }

    break;
  case davo_gl_e_pixel_type_monochrome8:
    davo_gl->framebuf.buffer.mono8_buffer =
        malloc(bufsize * sizeof(davo_gl_pixel_monochrome8_t));

    if (NULL == davo_gl->framebuf.buffer.mono8_buffer) {
      return (-1);
    }

    break;

  default:
    return (-1);
  }

  return (0);
}

void davo_gl_framebuffer_clear(struct davo_gl *davo_gl) {
  switch (davo_gl->framebuf.pixel_type) {
  case davo_gl_e_pixel_type_monochrome:
    memset((davo_gl->framebuf.buffer.mono_buffer), false,
           (davo_gl->height * davo_gl->width) *
               sizeof(davo_gl_pixel_monochrome_t));
    break;
  case davo_gl_e_pixel_type_monochrome8:
    memset((davo_gl->framebuf.buffer.mono8_buffer), false,
           (davo_gl->height * davo_gl->width) *
               sizeof(davo_gl_pixel_monochrome8_t));
    break;
  }
}

void davo_gl_set_pixel_to(struct davo_gl *davo_gl, int x, int y,
                          davo_gl_pixel_u pixel) {
  // simple bounds check
  if ((x < 0 || x >= davo_gl->width) || (y < 0 || y >= davo_gl->height)) {
    return;
  }
  switch (davo_gl->framebuf.pixel_type) {
  case davo_gl_e_pixel_type_monochrome:
    davo_gl->framebuf.buffer.mono_buffer[y * davo_gl->width + x] =
        pixel.mono_pixel;
    break;

  case davo_gl_e_pixel_type_monochrome8:
    davo_gl->framebuf.buffer.mono8_buffer[y * davo_gl->width + x] =
        pixel.mono8_pixel;
    break;
  }
}

void davo_gl_pixel_monochrome8_and(struct davo_gl *davo_gl, int buf_i,
                                   davo_gl_pixel_monochrome8_t p) {
  // bounds and type check
  if (davo_gl->framebuf.pixel_type != davo_gl_e_pixel_type_monochrome8) {
    return;
  }
  if (buf_i < 0 || buf_i > (davo_gl->height * davo_gl->width)) {
    return;
  }
  davo_gl->framebuf.buffer.mono8_buffer[buf_i] &= p;
}
void davo_gl_pixel_monochrome8_or(struct davo_gl *davo_gl, int buf_i,
                                  davo_gl_pixel_monochrome8_t p) {
  // bounds and type check
  if (davo_gl->framebuf.pixel_type != davo_gl_e_pixel_type_monochrome8) {
    return;
  }
  if (buf_i < 0 || buf_i > (davo_gl->height * davo_gl->width)) {
    return;
  }
  davo_gl->framebuf.buffer.mono8_buffer[buf_i] |= p;
}
