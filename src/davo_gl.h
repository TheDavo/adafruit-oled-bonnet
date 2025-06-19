#ifndef DAVO_GL_H
#define DAVO_GL_H
#include <stdbool.h>
#include <stdint.h>

#define davo_gl_print_prefix "[davo_gl]: "
#define davo_gl_print(fmt, args...) printf(davo_gl_print_prefix fmt, ##args)

enum davo_gl_pixel_type_e {
  davo_gl_e_pixel_type_monochrome,
};

typedef bool davo_gl_pixel_monochrome_t;

typedef struct davo_gl_framebuffer {
  enum davo_gl_pixel_type_e pixel_type;
  union {
    // other pixel types to go here later
    davo_gl_pixel_monochrome_t *mono_buffer;
  } buffer;
} davo_gl_framebuffer_t;

typedef union davo_gl_pixel {
  davo_gl_pixel_monochrome_t mono_pixel;
} davo_gl_pixel_u;

typedef struct davo_gl {
  int width;
  int height;
  davo_gl_framebuffer_t framebuf;
} davo_gl_t;

davo_gl_t *davo_gl_new_nullable();
int davo_gl_framebuffer_new(struct davo_gl *davo_gl,
                            enum davo_gl_pixel_type_e pixel_type, int width,
                            int height);

void davo_gl__framebuffer_free(struct davo_gl *davo_gl);
void davo_gl_free(struct davo_gl *davo_gl);

void davo_gl_set_pixel_to(struct davo_gl *davo_gl, int x, int y,
                          davo_gl_pixel_u pixel);

#endif
