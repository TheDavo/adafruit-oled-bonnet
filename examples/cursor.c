#include "../bonnet.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

typedef struct cursor_t {
  uint8_t edge_len;
  uint8_t topleft_x;
  uint8_t topleft_y;
} cursor_t;

typedef struct pixel_t {
  uint8_t x;
  uint8_t y;
  bool on;
} pixel_t;

void pixel_t_set(struct pixel_t *p, uint8_t x, uint8_t y, bool on) {
  p->on = on;
  p->x = x;
  p->y = y;
}
pixel_t *from_cursor_nullable(cursor_t c) {
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
      pixel_t_set(&cursor_repr[1],
                  c.topleft_x + c.edge_len - 1, c.topleft_y + c.edge_len - 1,
                  true);
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
  printf("pixel struct data:\nx = %d\ny = %d\non = %d\n", p.x, p.y, p.on);
}

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

  struct cursor_t my_cursor = {
      .edge_len = 10,
      .topleft_x = 0,
      .topleft_y = 0,
  };

  int cursor_size = 4 * (my_cursor.edge_len - 1);
  struct pixel_t *cursor_pixels = from_cursor_nullable(my_cursor);

  for (uint8_t i = 0; i < cursor_size; i++) {
    bonnet_action_write_to_pixel(&my_hat, cursor_pixels[i].x,
                                 cursor_pixels[i].y, cursor_pixels[i].on);
  }

  sleep(3);
  bonnet_write_cmd(my_hat, SET_DISP_OFF);
  bonnet_close(&my_hat);
  free(cursor_pixels);
  return 0;
}
