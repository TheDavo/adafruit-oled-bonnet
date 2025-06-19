#include "../src/bonnet.h"
#include "../src/davo_gl.h"
#include <stdio.h>
#include <unistd.h>

int main(void) {
  struct bonnet my_hat;
  int init_ret = bonnet_struct_init(&my_hat, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  bonnet_display_initialize(my_hat);
  bonnet_action_clear_display(&my_hat);

  davo_gl_t *davo = davo_gl_new_nullable();
  if (NULL == davo) {
    davo_gl_print("error allocating memory for davo_gl\n");
    return (-1);
  }

  if (davo_gl_framebuffer_new(davo, davo_gl_e_pixel_type_monochrome,
                              ssd1306_WIDTH, ssd1306_HEIGHT)) {
    davo_gl_print("error allocating new framebuffer");
    davo_gl_free(davo);
    return (-1);
  }

  davo_gl_pixel_u pixel;
  pixel.mono_pixel = true;
  for (int x = 0; x < davo->width; x++) {
    for (int y = 0; y < davo->height; y++) {

      davo_gl_set_pixel_to(davo, x, y, pixel);
      // davo_gl_print("mono_buffer[%d][%d] value %d\n", x, y,
      //               davo->framebuf.buffer.mono_buffer[y * davo->width + x]);
      bonnet_action_write_to_pixel(
          &my_hat, x, y,
          davo->framebuf.buffer.mono_buffer[y * davo->width + x]);
    }
  }

  davo_gl_free(davo);

  sleep(1);
  bonnet_action_clear_display(&my_hat);
  bonnet_close(&my_hat);
  
  
  return (0);
}
