#include "../src/bonnet.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h>

typedef struct cursor_info_t {
  uint8_t edge_len;
  uint8_t topleft_x;
  uint8_t topleft_y;
} cursor_info_t;

typedef struct pixel_t {
  uint8_t x;
  uint8_t y;
  bool on;
  bool visible;
} pixel_t;

typedef struct {
  uint8_t back_buffer[(HEIGHT * WIDTH) / PAGE_HEIGHT];
  uint8_t front_buffer[(HEIGHT * WIDTH) / PAGE_HEIGHT];
} framebuffer_t;

void framebuffer_set_back_data_at(framebuffer_t *fb, uint8_t x, uint8_t y,
                                  uint8_t value) {
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  fb->back_buffer[location] = value;
}

uint8_t framebuffer_get_back_data_at(framebuffer_t *fb, uint8_t x, uint8_t y) {
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  return fb->back_buffer[location];
}
uint8_t framebuffer_get_front_data_at(framebuffer_t *fb, uint8_t x, uint8_t y) {
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  return fb->front_buffer[location];
}
void framebuffer_set_front_data_at(framebuffer_t *fb, uint8_t x, uint8_t y,
                                   uint8_t value) {
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  fb->front_buffer[location] = value;
}

void framebuffer_update_backbuffer(framebuffer_t *fb, pixel_t *cursor,
                                   int cursor_size) {
  // memset(fb->back_buffer, 0, FRAMEBUFFER_SIZE);
  for (int i = 0; i < cursor_size; i++) {
    if (cursor[i].visible) {
      uint8_t frame_byte =
          framebuffer_get_back_data_at(fb, cursor[i].x, cursor[i].y);
      uint8_t new_frame_byte = 0;
      if (cursor[i].on) {
        new_frame_byte = frame_byte | (1 << (cursor[i].y % 8));
      } else {
        new_frame_byte = frame_byte & (0 << (cursor[i].y % 8));
      }
      framebuffer_set_back_data_at(fb, cursor[i].x, cursor[i].y,
                                   new_frame_byte);
    }
  }
}

void framebuffer_set_back_to_front(framebuffer_t *fb) {
  memcpy(fb->front_buffer, fb->back_buffer, FRAMEBUFFER_SIZE);
  memset(fb->back_buffer, 0, FRAMEBUFFER_SIZE);
}

void framebuffer_buffers_reset(framebuffer_t *fb) {
  memset(fb->back_buffer, 0, FRAMEBUFFER_SIZE);
  memset(fb->front_buffer, 0, FRAMEBUFFER_SIZE);
}

bool framebuffer_has_delta(framebuffer_t fb) {
  for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
    if (fb.back_buffer[i] != fb.front_buffer[i]) {
      return true;
    }
  }
  return false;
}

bool pixel_t_bounds_check(struct pixel_t p) {
  return (p.x >= 0 && p.x < WIDTH) && (p.y >= 0 && p.y < HEIGHT);
}
bool pixel_t_bounds_check_limits(struct pixel_t p, uint8_t x_lim,
                                 uint8_t y_lim) {
  return (p.x >= 0 && p.x < x_lim) && (p.y >= 0 && p.y < y_lim);
}

void pixel_t_set(struct pixel_t *p, uint8_t x, uint8_t y, bool on) {
  p->on = on;
  p->x = x;
  p->y = y;
  p->visible = pixel_t_bounds_check(*p);
}

pixel_t *cursor_from_cursorinfo_nullable(cursor_info_t c) {
  if (c.edge_len == 0) {
    return NULL;
  }
  if (c.edge_len == 1) {
    pixel_t *r = malloc(sizeof(struct pixel_t));
    if (r == NULL) {
      printf("error mallocing memory for pixels\n");
      return NULL;
    }
    pixel_t_set(r, c.topleft_x, c.topleft_y, true);
    return r;
  }

  int cursor_size = 4 * (c.edge_len - 1);

  pixel_t *cursor_repr = malloc(sizeof(struct pixel_t) * cursor_size);
  if (cursor_repr == NULL) {
    printf("error mallocing memory for pixels\n");
    return NULL;
  }
  for (uint8_t i = 0; i < c.edge_len; i++) {
    // handle corners
    if (i == 0) {
      pixel_t_set(&cursor_repr[0], c.topleft_x + i, c.topleft_y, true);
      pixel_t_set(&cursor_repr[1], c.topleft_x + c.edge_len - 1,
                  c.topleft_y + c.edge_len - 1, true);
    } else {
      // handle edges
      pixel_t_set(&cursor_repr[2 * i], c.topleft_x + i, c.topleft_y, true);
      pixel_t_set(&cursor_repr[2 * i + 1], c.topleft_x, c.topleft_y + i, true);

      pixel_t_set(&cursor_repr[cursor_size - 2 * i],
                  c.topleft_x + c.edge_len - 1,
                  c.topleft_y + c.edge_len - 1 - i, true);
      pixel_t_set(&cursor_repr[cursor_size - 2 * i + 1],
                  c.topleft_x + c.edge_len - 1 - i,
                  c.topleft_y + c.edge_len - 1, true);
    }
  }
  return cursor_repr;
}

void pixel_t_print(struct pixel_t p) {
  printf("pixel struct data:\nx = %d\ny = %d\non = %d\nvisible = %d", p.x, p.y,
         p.on, p.visible);
}

void cursor_action_draw(struct pixel_t *cursor_pixels, bonnet *b,
                        uint8_t edge_len) {
  int cursor_size = 4 * (edge_len - 1);

  for (int i = 0; i < cursor_size; i++) {
    if (cursor_pixels[i].visible) {
      bonnet_action_write_to_pixel(b, cursor_pixels[i].x, cursor_pixels[i].y,
                                   cursor_pixels[i].on);
    }
  }
}

void cursor_update_all_pixel_pos(struct pixel_t *cursor_pixels, int cursor_size,
                                 uint8_t x_move, uint8_t y_move) {
  for (int i = 0; i < cursor_size; i++) {
    pixel_t_set(&cursor_pixels[i], cursor_pixels[i].x + x_move,
                cursor_pixels[i].y + y_move, cursor_pixels[i].on);
  }
}

int main(int argc, char **argv) {
  struct bonnet my_hat;
  framebuffer_t fb;
  int init_ret = bonnet_struct_init(&my_hat, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  framebuffer_buffers_reset(&fb);
  bonnet_display_initialize(my_hat);

  int edge_len_input = 10; // default
  uint8_t move_delta = 5;
  if (argc == 2) {
    edge_len_input = atoi(argv[1]);
    if (edge_len_input < 2) {
      edge_len_input = 2;
    }
    edge_len_input = (uint8_t)edge_len_input;
  }
  if (argc == 3) {
    edge_len_input = atoi(argv[1]);
    if (edge_len_input < 2) {
      edge_len_input = 2;
    }
    edge_len_input = (uint8_t)edge_len_input;
    move_delta = (uint8_t)atoi(argv[2]);
  }

  struct cursor_info_t my_cursor = {
      .edge_len = edge_len_input,
      .topleft_x = 0,
      .topleft_y = 0,
  };

  int cursor_size = 4 * (my_cursor.edge_len - 1);
  framebuffer_buffers_reset(&fb);
  // int cursor_size = 1;
  struct pixel_t *cursor_pixels = cursor_from_cursorinfo_nullable(my_cursor);

  for (int i = 0; i < cursor_size; i++) {
    if (cursor_pixels[i].visible) {
      bonnet_action_write_to_pixel(&my_hat, cursor_pixels[i].x,
                                   cursor_pixels[i].y, cursor_pixels[i].on);
    }
  }
  framebuffer_update_backbuffer(&fb, cursor_pixels, cursor_size);
  framebuffer_set_back_to_front(&fb);
  int btn_a_pressed;
  int btn_b_pressed;
  int btn_l_pressed;
  int btn_r_pressed;
  int btn_u_pressed;
  int btn_d_pressed;
  int values[CONST_NUM_BUTTONS];
  bool direction_pressed = false;
  bool erase = false;
  uint8_t x_move = 0, y_move = 0;
  uint8_t set_column = 0x21;
  uint8_t set_page = 0x22;
  uint8_t cmds[] = {set_column, 0, WIDTH - 1, set_page, 0, 7};
  bonnet_write_multi_cmd(my_hat, cmds, sizeof(cmds));

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
    }
    if (btn_l_pressed == 0) {
      x_move = -move_delta;
      direction_pressed = true;
    }
    if (btn_r_pressed == 0) {
      x_move = +move_delta;
      direction_pressed = true;
    }
    if (btn_u_pressed == 0) {
      y_move = -move_delta;
      direction_pressed = true;
    }
    if (btn_d_pressed == 0) {
      y_move = +move_delta;
      direction_pressed = true;
    }
    if (direction_pressed) {
      // bonnet_action_clear_display(&my_hat);
      cursor_update_all_pixel_pos(cursor_pixels, cursor_size, x_move, y_move);
      framebuffer_update_backbuffer(&fb, cursor_pixels, cursor_size);
      // uint8_t new_x;
      // uint8_t page;
      // for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
      //   if (fb.back_buffer[i] != fb.front_buffer[i]) {
      //     new_x = (uint8_t)(i % WIDTH);
      //     page = (uint8_t)(i / WIDTH);
      //     bonnet_action_write_to_segment(&my_hat, page, new_x,
      //                                   fb.back_buffer[i]);

      //   }
      // }
      framebuffer_set_back_to_front(&fb);
      bonnet_write_multi_data(my_hat, fb.front_buffer, 1024);
      direction_pressed = false;
      x_move = 0;
      y_move = 0;
    }

    // usleep(10000);
  }

  bonnet_write_cmd(my_hat, SET_DISP_OFF);
  bonnet_close(&my_hat);
  free(cursor_pixels);
  return 0;
}
