#include "bonnet.h"
#include <fcntl.h>
#include <gpiod.h>
#include <linux/gpio.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void bonnet_close(struct bonnet *b) {
  gpiod_line_release_bulk(&(b->buttons));
  gpiod_chip_close(b->gpio_chip);
  close(b->i2cfd);
}

int bonnet__init_gpio_lines(struct bonnet *b) {
  // help from https://lloydrochester.com/post/hardware/libgpiod-event-rpi/
  // to initialize multiple lines

  if (b->gpio_chip == NULL) {
    printf("GPIO chip not iniialized\n");
    return (-1);
  }

  unsigned int offsets[] = {CONST_BUTTON_A, CONST_BUTTON_B, CONST_BUTTON_C,
                            CONST_BUTTON_D, CONST_BUTTON_L, CONST_BUTTON_R,
                            CONST_BUTTON_U};

  int err;
  err = gpiod_chip_get_lines(b->gpio_chip, offsets, CONST_NUM_BUTTONS,
                             &(b->buttons));
  if (err) {
    printf("Error getting bulk lines\n");
    return (-1);
  }

  err = gpiod_line_request_bulk_input_flags(
      &(b->buttons), CONST_CONSUMER, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
  if (err) {
    printf("Error line request bulk pull up\n");
    return (-1);
  }

  return (0);
}

int bonnet_struct_init(struct bonnet *b, uint8_t bonnet_i2c_addr) {

  char *i2c_filename = "/dev/i2c-1";
  char *gpio_chipname = "gpiochip0";
  b->i2c_addr = bonnet_i2c_addr;
  printf("opening i2c\n");
  if ((b->i2cfd = open(i2c_filename, O_RDWR)) < 0) {
    printf("Error opening i2c file descriptor\n");
    return (-1);
  }

  printf("setting up i2c\n");
  if (ioctl(b->i2cfd, I2C_SLAVE, bonnet_i2c_addr)) {
    printf("Error setting I2C slave addr\n");
    close(b->i2cfd);
    return (-1);
  }

  printf("opening gpio\n");
  if ((b->gpio_chip = gpiod_chip_open_by_name(gpio_chipname)) == NULL) {
    printf("Error opening gpiod chip name %s\n", gpio_chipname);

    // since at this point only the i2c fd is open, close that
    close(b->i2cfd);
    return (-1);
  }

  // everything opened OK, can also request gpio lines for the buttons
  bonnet__init_gpio_lines(b);

  memset(b->framebuffer, 0, HEIGHT * WIDTH / PAGE_HEIGHT);
  return (0);
}

int bonnet_write_cmd(const struct bonnet b, uint8_t cmd) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x00 sets the Co bit to 0, and the D/C# bit to 0
  // to set the next set of bytes as command bytes and
  // not data bytes
  uint8_t cmd_buffer[2] = {0x00, cmd};
  int len_buffer = sizeof(cmd_buffer);
  // printf("[bonnet_write_cmd] sizeof(buffer)= %d\n", len_buffer);
  if (write(b.i2cfd, cmd_buffer, len_buffer) != len_buffer) {
    printf("Error writing command buffer to I2C\n");
    return (-1);
  }
  return (0);
}

int bonnet_write_data(const struct bonnet b, uint8_t data) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x40 sets the Co bit to 0, and the D/C# bit to 1 to set the next set of
  // bytes as data not data bytes
  //
  // if the D/C# bit is set to logic 1, it defines the following data byte as
  // a data which will be stored at the Graphics Display Data RAM
  uint8_t data_buffer[2] = {0x40, data};
  // printf("[bonnet_write_data] data: %x\n", data);
  int len_buffer = sizeof(data_buffer);
  if (write(b.i2cfd, data_buffer, len_buffer) != len_buffer) {
    printf("Error writing data buffer to I2C\n");
    return (-1);
  }
  return (0);
}

int bonnet_write_multi_cmd(const struct bonnet b, uint8_t cmds[],
                           int len_cmds) {
  int write_ret = 0;
  for (int i = 0; i < len_cmds; i++) {
    write_ret = bonnet_write_cmd(b, cmds[i]);
    if (write_ret == -1) {
      return (-1);
    }
  }
  return (0);
}

int bonnet_write_multi_data(struct bonnet b, uint8_t data[],
                            int lenbytes_data) {
  uint8_t *buffer = malloc(lenbytes_data + (sizeof(uint8_t)));
  buffer[0] = 0x40;
  memcpy(buffer + 1, data, lenbytes_data);
  int ret_val = write(b.i2cfd, buffer, lenbytes_data + sizeof(uint8_t));
  free(buffer);
  return ret_val;
}

int bonnet_poweron(struct bonnet b) {
  // TODO: For the bonnet, no reset commands are used to to AXP830 chip
  // monitoring the device VCC, for an agnostic driver (which this will
  // eventually become), this function needs to be correctly implemented
  return bonnet_write_cmd(b, SET_DISP_ON);
}

int bonnet_write_to_page(struct bonnet b, uint8_t page, uint8_t start_col,
                         uint8_t end_col, uint8_t data[], int lenbytes_data) {
  // set GDDRAM page start address
  bonnet_write_cmd(b, 0xB0 | page);
  bonnet_write_cmd(b, start_col);
  bonnet_write_cmd(b, 0x10 | end_col);

  bonnet_write_multi_data(b, data, lenbytes_data);
  return (0);
}

int bonnet_display_initialize(struct bonnet b) {
  // initialization commands copied from the adafruit bonnet repo
  // https://github.com/adafruit/Adafruit_CircuitPython_SSD1306/blob/main/adafruit_ssd1306.py
  bonnet_write_cmd(b, SET_DISP_OFF);
  uint8_t mux_ratio_cmds[] = {SET_MUX_RATIO, HEIGHT - 1};
  bonnet_write_multi_cmd(b, mux_ratio_cmds, 2);
  uint8_t disp_offset_cmds[] = {SET_DISP_OFFSET, 0x00};
  bonnet_write_multi_cmd(b, disp_offset_cmds, 2);
  bonnet_write_cmd(b, SET_DISP_START_LINE);
  bonnet_write_cmd(b, SET_SEG_REMAP | 0x01);
  bonnet_write_cmd(b, SET_COM_OUT_DIR | 0x08);
  uint8_t com_pin_cfg_cmds[] = {SET_COM_PIN_CFG,
                                WIDTH > (HEIGHT * 2) ? 0x02 : 0x12};
  bonnet_write_multi_cmd(b, com_pin_cfg_cmds, 2);
  uint8_t contrast_cmds[] = {SET_CONTRAST, 0x0F};
  bonnet_write_multi_cmd(b, contrast_cmds, 2);
  bonnet_write_cmd(b, SET_ENTIRE_RAM);
  uint8_t disp_clk_div_cmds[] = {SET_DISP_CLK_DIV, 0x80};
  bonnet_write_multi_cmd(b, disp_clk_div_cmds, 2);
  uint8_t precharge_cmds[] = {SET_PRECHARGE, 0xF1};
  bonnet_write_multi_cmd(b, precharge_cmds, 2);
  uint8_t mem_cmds[] = {SET_MEM_ADDR, 0b00};
  bonnet_write_multi_cmd(b, mem_cmds, 2);
  uint8_t vcom_desel_cmds[] = {SET_VCOM_DESEL, 0x30}; // no external Vcc
  bonnet_write_multi_cmd(b, vcom_desel_cmds, 2);
  bonnet_write_cmd(b, SET_NORM_INV);
  uint8_t iref_select_cmds[] = {SET_IREF_SELECT, 0x30};
  bonnet_write_multi_cmd(b, iref_select_cmds, 2);
  uint8_t chargepump_cmds[] = {SET_CHARGE_PUMP, 0x14};
  bonnet_write_multi_cmd(b, chargepump_cmds, 2);
  usleep(1000);
  bonnet_write_cmd(b, SET_DISP_ON);

  return (0);
}

/**
 * bonnet__get_framebuffer_data_at returns the byte that the pixel (x,y) would
 * be at
 *
 * Bonnet's framebuffer is segmented with pages and not individual pixels, and
 * the framebuffer mimics that structure
 *
 * This function is correctly used by bonnet_action_write_to_pixel and should
 * not be called on its own.
 */
uint8_t bonnet__get_framebuffer_data_at(struct bonnet b, uint8_t x, uint8_t y) {
  // column  y 00 01 02 03 04 and so on...
  // page 0  0|01|  |  |  |  |
  // page 0  1|02|  |  |  |  |
  // page 0  2|04|  |  |  |  |
  // page 0  3|08|  |  |  |  |
  // page 0  4|10|  |  |  |  |
  // page 0  5|20|  |  |  |  |
  // page 0  6|40|  |  |  |  |
  // page 0  7|80|  |  |  |  |
  // page 1  8|00|  |  |  |  |
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  return b.framebuffer[location];
}

/**
 * bonnet__set_framebuffer_data_at sets the byte value at the page and column
 * determined by where a pixel at (x,y) would be at
 *
 * This function is correctly used by bonnet_action_write_to_pixel and should
 * not be called on its own.
 */

void bonnet__set_framebuffer_data_at(struct bonnet *b, uint8_t x, uint8_t y,
                                     uint8_t value) {
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  b->framebuffer[location] = value;
}

/**
 * bonnet_action_write_to_pixel sets or unsets a pixel at the x,y coordinates.
 *
 * This is an action function and will update the rendering on the display.
 */
void bonnet_action_write_to_pixel(struct bonnet *b, uint8_t x, uint8_t y,
                                  bool set) {
  // simple clamping for now, allows wrapping
  x = x % (WIDTH);
  y = y % (HEIGHT);
  uint8_t page = (y / 8);

  uint8_t col_byte = bonnet__get_framebuffer_data_at(*b, x, y);
  uint8_t new_col_byte = 0;
  if (set) {
    new_col_byte = col_byte | (1 << (y % 8));
  } else {
    new_col_byte = col_byte & (0 << (y % 8));
  }
  bonnet__set_framebuffer_data_at(b, x, y, new_col_byte);
  if (new_col_byte != col_byte) {
    // printf("writing to page %d -> (%3d,%2d) with value %d -> %d\n", page, x,
    // y,
    //        col_byte, new_col_byte);
  }

  uint8_t set_column = 0x21;
  uint8_t set_page = 0x22;
  uint8_t cmds[] = {set_column, x, x, set_page, page, page};
  uint8_t len_cmds = sizeof(cmds) / sizeof(uint8_t);
  bonnet_write_multi_cmd(*b, cmds, len_cmds);
  bonnet_write_data(*b, new_col_byte);
}

/**
 * bonnet_action_clear_display clears the framebuffer and updates the display.
 * This function _will_ overwrite the internal framebuffer and set it all
 * to zero.
 *
 * This is an action function and will update the rendering on the display.
 */
void bonnet_action_clear_display(struct bonnet *b) {
  uint8_t cmds[] = {0x21, 0, WIDTH - 1, 0x22, 0, 7, SET_CONTRAST, 0x00};
  uint8_t len_cmds = sizeof(cmds) / sizeof(uint8_t);
  bonnet_write_multi_cmd(*b, cmds, len_cmds);
  memset(b->framebuffer, 0x00, sizeof(b->framebuffer));
  bonnet_write_multi_data(*b, b->framebuffer, sizeof(b->framebuffer));
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
      --x_at;
      direction_pressed = true;
    }
    if (btn_r_pressed == 0) {
      ++x_at;
      direction_pressed = true;
    }
    if (btn_u_pressed == 0) {
      --y_at;
      direction_pressed = true;
    }
    if (btn_d_pressed == 0) {
      ++y_at;
      direction_pressed = true;
    }
    if (direction_pressed) {
      // printf("x, y: %d, %d\n", x_at, y_at);
      bonnet_action_write_to_pixel(&my_hat, x_at, y_at, !erase);
      direction_pressed = false;
    }

    usleep(10000);
  }
  // bonnet_clear_display(my_hat);
  bonnet_write_cmd(my_hat, SET_DISP_OFF);
  bonnet_close(&my_hat);
  return 0;
}
