#include "../src/bonnet.h"
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
  int btn_a_pressed;
  int btn_b_pressed;
  int btn_l_pressed;
  int btn_r_pressed;
  int btn_u_pressed;
  int btn_d_pressed;
  int values[CONST_NUM_BUTTONS];
  uint8_t x_at = WIDTH / 2;
  uint8_t y_at = HEIGHT / 2;
  bool direction_pressed = false;
  bool erase = false;
  while (true) {
    gpiod_line_get_value_bulk(&(my_hat.buttons), values);
    // unsigned int offsets[] = {CONST_BUTTON_A, CONST_BUTTON_B, CONST_BUTTON_C,
    //                           CONST_BUTTON_D, CONST_BUTTON_L, CONST_BUTTON_R,
    //                           CONST_BUTTON_U};
    btn_a_pressed = values[0];
    btn_b_pressed = values[1];
    btn_d_pressed = values[3];
    btn_l_pressed = values[4];
    btn_r_pressed = values[5];
    btn_u_pressed = values[6];

    // since there is a pull-up, these are default 1
    // a 0 would indicate pressed
    if (btn_a_pressed == 0 && btn_b_pressed == 0) {
      printf("Button A&B pressed, exiting\n");
      break;
    }

    if (btn_a_pressed == 0) {
      bonnet_action_clear_display(&my_hat);
    }
    if (btn_b_pressed == 0) {
      erase = !erase;
      printf("erase value: %d\n", erase);
    }
    if (btn_l_pressed == 0) {
      if (x_at == 0)
        x_at = 0;
      else
        --x_at;
      direction_pressed = true;
    }
    if (btn_r_pressed == 0) {
      if (x_at >= WIDTH - 1)
        x_at = WIDTH - 1;
      else
        ++x_at;
      direction_pressed = true;
    }
    if (btn_u_pressed == 0) {
      if (y_at == 0)
        y_at = 0;
      else
        --y_at;
      direction_pressed = true;
    }
    if (btn_d_pressed == 0) {
      if (y_at >= HEIGHT - 1)
        y_at = HEIGHT - 1;
      else
        ++y_at;
      direction_pressed = true;
    }
    if (direction_pressed) {
      printf("x, y: %d, %d\n", x_at, y_at);
      bonnet_action_write_to_pixel(&my_hat, x_at, y_at, !erase);
      direction_pressed = false;
    }

    usleep(10000);
  }
  // bonnet_clear_display(my_hat);
  bonnet_set_display_off(my_hat);
  bonnet_close(&my_hat);
  return 0;
}
