#include "bonnet.h"
#include "ssd1306.h"
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
  ssd1306_free(b->ssd);
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
  ssd1306_struct_init(&b->ssd, bonnet_i2c_addr, i2c_filename);

  printf("opening gpio\n");
  if ((b->gpio_chip = gpiod_chip_open_by_name(gpio_chipname)) == NULL) {
    printf("Error opening gpiod chip name %s\n", gpio_chipname);

    // since at this point only the i2c fd is open, close that
    close(b->ssd.i2c_addr);
    return (-1);
  }

  // everything opened OK, can also request gpio lines for the buttons
  bonnet__init_gpio_lines(b);

  memset(b->framebuffer, 0, FRAMEBUFFER_SIZE);
  printf("end init\n");
  return (0);
}

int bonnet_write_cmd(struct bonnet b, uint8_t cmd) {
  return ssd1306_write_cmd(b.ssd, cmd);
}

int bonnet_write_data(const struct bonnet b, uint8_t data) {
  return ssd1306_write_data(b.ssd, data);
}

int bonnet_write_multi_cmd(const struct bonnet b, uint8_t cmds[],
                           int len_cmds) {
  int write_ret = 0;
  for (int i = 0; i < len_cmds; i++) {
    write_ret = ssd1306_write_cmd(b.ssd, cmds[i]);
    if (write_ret == -1) {
      return (-1);
    }
  }
  return (0);
}

int bonnet_write_multi_data(struct bonnet b, uint8_t data[],
                            int lenbytes_data) {
  // uint8_t *buffer = malloc(lenbytes_data + (sizeof(uint8_t)));
  // buffer[0] = 0x40;
  // memcpy(buffer + 1, data, lenbytes_data);
  // int ret_val = write(b.ssd.i2cfd, buffer, lenbytes_data + sizeof(uint8_t));
  int ret_val = ssd1306_write_data_multi(b.ssd, data, lenbytes_data);
  return ret_val;
}

int bonnet_poweron(struct bonnet b) {
  // TODO: For the bonnet, no reset commands are used to to AXP830 chip
  // monitoring the device VCC, for an agnostic driver (which this will
  // eventually become), this function needs to be correctly implemented
  return ssd1306_write_cmd(b.ssd, SET_DISP_ON);
}

int bonnet_write_to_page(struct bonnet b, uint8_t page, uint8_t start_col,
                         uint8_t end_col, uint8_t data[], int lenbytes_data) {
  // set GDDRAM page start address
  ssd1306_write_cmd(b.ssd, 0xB0 | page);
  ssd1306_write_cmd(b.ssd, start_col);
  ssd1306_write_cmd(b.ssd, 0x10 | end_col);

  bonnet_write_multi_data(b, data, lenbytes_data);
  return (0);
}

int bonnet_display_initialize(struct bonnet b) {
  // initialization commands copied from the adafruit bonnet repo
  // https://github.com/adafruit/Adafruit_CircuitPython_SSD1306/blob/main/adafruit_ssd1306.py
  ssd1306_write_cmd(b.ssd, SET_DISP_OFF);
  uint8_t mux_ratio_cmds[] = {SET_MUX_RATIO, HEIGHT - 1};
  ssd1306_write_cmd_multi(b.ssd, mux_ratio_cmds, 2);
  uint8_t disp_offset_cmds[] = {SET_DISP_OFFSET, 0x00};
  ssd1306_write_cmd_multi(b.ssd, disp_offset_cmds, 2);
  ssd1306_write_cmd(b.ssd, SET_DISP_START_LINE);
  ssd1306_write_cmd(b.ssd, SET_SEG_REMAP | 0x01);
  ssd1306_write_cmd(b.ssd, SET_COM_OUT_DIR | 0x08);
  uint8_t com_pin_cfg_cmds[] = {SET_COM_PIN_CFG,
                                WIDTH > (HEIGHT * 2) ? 0x02 : 0x12};
  ssd1306_write_cmd_multi(b.ssd, com_pin_cfg_cmds, 2);
  uint8_t contrast_cmds[] = {SET_CONTRAST, 0x03};
  ssd1306_write_cmd_multi(b.ssd, contrast_cmds, 2);
  ssd1306_write_cmd(b.ssd, SET_ENTIRE_RAM);
  uint8_t disp_clk_div_cmds[] = {SET_DISP_CLK_DIV, 0x80};
  ssd1306_write_cmd_multi(b.ssd, disp_clk_div_cmds, 2);
  uint8_t precharge_cmds[] = {SET_PRECHARGE, 0xF1};
  ssd1306_write_cmd_multi(b.ssd, precharge_cmds, 2);
  uint8_t mem_cmds[] = {SET_MEM_ADDR, 0b00};
  ssd1306_write_cmd_multi(b.ssd, mem_cmds, 2);
  uint8_t vcom_desel_cmds[] = {SET_VCOM_DESEL, 0x30}; // no external Vcc
  ssd1306_write_cmd_multi(b.ssd, vcom_desel_cmds, 2);
  ssd1306_write_cmd(b.ssd, SET_NORM_INV);
  uint8_t iref_select_cmds[] = {SET_IREF_SELECT, 0x30};
  ssd1306_write_cmd_multi(b.ssd, iref_select_cmds, 2);
  uint8_t chargepump_cmds[] = {SET_CHARGE_PUMP, 0x14};
  ssd1306_write_cmd_multi(b.ssd, chargepump_cmds, 2);
  usleep(1000);
  ssd1306_write_cmd(b.ssd, SET_DISP_ON);
  bonnet_action_clear_display(&b);

  return (0);
}

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

void bonnet__set_framebuffer_data_at(struct bonnet *b, uint8_t x, uint8_t y,
                                     uint8_t value) {
  int location = (y / PAGE_HEIGHT) * WIDTH + x;
  b->framebuffer[location] = value;
}

void bonnet_action_write_to_pixel(struct bonnet *b, uint8_t x, uint8_t y,
                                  bool set) {
  uint8_t page = (y / PAGE_HEIGHT);

  uint8_t col_byte = bonnet__get_framebuffer_data_at(*b, x, y);
  uint8_t new_col_byte = 0;
  if (set) {
    new_col_byte = col_byte | (1 << (y % 8));
  } else {
    new_col_byte = col_byte & (0 << (y % 8));
  }
  bonnet__set_framebuffer_data_at(b, x, y, new_col_byte);

  ssd1306_write_data_to_segment(b->ssd, page, x, new_col_byte);

}

void bonnet_display_framebuffer(struct bonnet b) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      printf("%d", b.framebuffer[i * WIDTH + j]);
    }
    printf("\n");
  }
}
void bonnet_action_write_to_column(struct bonnet *b, uint8_t page, uint8_t col,
                                  uint8_t new_data) {
  uint8_t set_column = 0x21;
  uint8_t set_page = 0x22;
  uint8_t cmds[] = {set_column, col, col, set_page, page, page};
  ssd1306_write_cmd_multi(b->ssd, cmds, sizeof(cmds));
  ssd1306_write_data(b->ssd, new_data);
}

void bonnet_action_clear_display(struct bonnet *b) {
  uint8_t set_column = 0x21;
  uint8_t set_page = 0x22;
  uint8_t cmds[] = {set_column, 0, WIDTH - 1, set_page, 0, 7};
  ssd1306_write_cmd_multi(b->ssd, cmds, sizeof(cmds));
  memset(b->framebuffer, 0x00, sizeof(b->framebuffer));
  ssd1306_write_data_multi(b->ssd, b->framebuffer, sizeof(b->framebuffer));
}
