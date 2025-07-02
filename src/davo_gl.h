#ifndef DAVO_GL_H
#define DAVO_GL_H
#include <stdbool.h>
#include <stdint.h>

#define davo_gl_print_prefix "[davo_gl]: "
#define davo_gl_print(fmt, args...) printf(davo_gl_print_prefix fmt, ##args)

enum davo_gl_pixel_type_e {
  davo_gl_e_pixel_type_monochrome,
  davo_gl_e_pixel_type_monochrome8,
};

typedef bool davo_gl_pixel_monochrome_t;
typedef uint8_t davo_gl_pixel_monochrome8_t;

typedef struct davo_gl_framebuffer {
  enum davo_gl_pixel_type_e pixel_type;
  union {
    // other pixel types to go here later
    davo_gl_pixel_monochrome_t *mono_buffer;
    davo_gl_pixel_monochrome8_t *mono8_buffer;
  } buffer;
} davo_gl_framebuffer_t;

typedef union davo_gl_pixel {
  davo_gl_pixel_monochrome_t mono_pixel;
  davo_gl_pixel_monochrome8_t mono8_pixel;
} davo_gl_pixel_u;

typedef struct davo_gl {
  int width;
  int height;
  davo_gl_framebuffer_t framebuf;
} davo_gl_t;

/**
 * davo_gl_new_nullable allocates memory for a new davo_gl type
 * this function can return NULL and therefore needs proper error checking
 * after its use
 *
 */
davo_gl_t *davo_gl_new_nullable();

/**
 * davo_gl_framebuffer_new allocates memory for a framebuffer of a particular
 * pixel type with a width `width` and height `height`
 *
 * This function does not allocate a 2-dimensional array, but a 1-dimensional
 * array of size width times height
 *
 */
int davo_gl_framebuffer_new(struct davo_gl *davo_gl,
                            enum davo_gl_pixel_type_e pixel_type, int width,
                            int height);

/**
 * davo_gl_framebuffer_clear clears the framebuffer value by setting all the
 * framebuffer values to an `off` pixel
 *
 */
void davo_gl_framebuffer_clear(struct davo_gl *davo_gl);

/**
 * davo_gl__framebuffer_free is an INTERNAL function, and frees the memory of
 * the internal framebuffer struct inside the davo_gl type. This function should
 * not be called on its own, as it is called when `davo_gl_free` is called
 *
 */
void davo_gl__framebuffer_free(struct davo_gl *davo_gl);

/**
 * davo_gl_free frees the allocated memory of the davo_gl struct. This function
 * also frees the framebuffer memory
 *
 */
void davo_gl_free(struct davo_gl *davo_gl);

/**
 * davo_gl_set_pixel_to sets the pixel value at `x`, `y` to the value of
 * `pixel`
 *
 * The caller of this function must pass in the correct pixel type to the
 * function, otherwise undefined behavior may happen
 *
 * This function performs a simple bonud check on the `x` and `y` inputs, and
 * returns early when either input is out of range
 *
 */
void davo_gl_set_pixel_to(struct davo_gl *davo_gl, int x, int y,
                          davo_gl_pixel_u pixel);

void davo_gl_pixel_monochrome8_and(struct davo_gl *davo_gl, int buf_i,
                                   davo_gl_pixel_monochrome8_t p);
void davo_gl_pixel_monochrome8_or(struct davo_gl *davo_gl, int buf_i,
                                  davo_gl_pixel_monochrome8_t p);

void davo_gl_draw_line(struct davo_gl *davo_gl, int x1, int y1, int x2, int y2,
                       davo_gl_pixel_u pixel);
void davo_gl_draw_rect(struct davo_gl *davo_gl, int top_l, int length,
                       int height, davo_gl_pixel_u pixel, bool filled);
#endif
