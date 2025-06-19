#include "davo_gl.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

davo_gl_t *davo_gl_new_nullable() {
  davo_gl_t *d_gl = malloc(sizeof(davo_gl_t));
  return d_gl;
}

void davo_gl_free(struct davo_gl *davo_gl) {
  if (NULL != davo_gl) {
    switch (davo_gl->framebuf.pixel_type) {
    case davo_gl_e_pixel_type_monochrome:
      free(davo_gl->framebuf.buffer.mono_buffer);
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
  default:
    return (-1);
  }

  return (0);
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
  }
}

uint8_t *davo_gl_uint8t_arr_from_monochrome_nullable(struct davo_gl davo_gl) {
  uint8_t bit_height = 8;
  uint8_t new_height = davo_gl.height / bit_height;
  uint8_t *as_uint8t =
      malloc(sizeof(davo_gl_pixel_monochrome_t) * davo_gl.width * new_height);

  if (NULL == as_uint8t) {
    return NULL;
  }

  for (int loc = 0; loc < davo_gl.width * new_height; loc++) {
    uint8_t merged_pixel = 0;
    for (uint8_t bit = 0; bit < bit_height; bit++) {
      uint8_t mono_buf_loc = loc + (bit * davo_gl.width);
      merged_pixel |=
          (davo_gl.framebuf.buffer.mono_buffer[mono_buf_loc] << bit);
    }
    as_uint8t[loc] = merged_pixel;
  }

  return as_uint8t;
}
