#include "./src/bonnet.h"
#include <stdio.h>
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

  int i = 0;

  while (i < 100) {

    int state_up = bonnet_button_get_state(b, bonnet_e_button_up);
    int state_down = bonnet_button_get_state(b, bonnet_e_button_down);
    int state_left = bonnet_button_get_state(b, bonnet_e_button_left);
    int state_right = bonnet_button_get_state(b, bonnet_e_button_right);

    // print the buttons in a d-pad style
    //
    printf("\t%s\n%s\t%s\n\t%s\n",
           bonnet_button_state_into_str(state_up),
           bonnet_button_state_into_str(state_left),
           bonnet_button_state_into_str(state_right),
           bonnet_button_state_into_str(state_down));

    i++;
    // 100 msec
    usleep(100 * 1000);
  }

  bonnet_close(&b);
}
