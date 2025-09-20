

#include "../src/bonnet.h"
#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"
#include "../src/uic/components.h"
#include "../src/uic/cursor.h"

#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  struct bonnet b;
  int init_ret = bonnet_struct_init(&b, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  bonnet_display_initialize(b);
  bonnet_action_clear_display(&b);

  uic_cursor_attr_t cursor_attr = {.origin = {
                                       .x = 10,
                                       .y = 10,
                                   }};

  uic_t *cursor = uic_cursor_new(&cursor_attr);

  bonnet_e_button_state states[7];
  // pressing A and B quits the program
  while (states[0] != bonnet_e_button_state_down &&
         states[1] != bonnet_e_button_state_down) {
    bonnet_button_get_states(b, states);
    ssd1306_fb_clear_buffer(b.ssd.framebuf, false);

    if (states[BONNET_BUTTON_IDX_RIGHT] ==  bonnet_e_button_state_down) {
      uic_cursor_position_update_relative(cursor, 10, 0);
    }

    cursor->draw(b.ssd.framebuf, cursor->attr);
    ssd1306_write_framebuffer_all(b.ssd);
  }
  bonnet_set_display_off(b);
  bonnet_close(&b);
}
