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

// display driver constants
#define SET_CONTRAST 0x81
#define SET_ENTIRE_ON 0xA5
#define SET_ENTIRE_RAM 0xA4
#define SET_NORM_INV 0xA6
#define SET_DISP_ON 0xAF
#define SET_DISP_OFF 0xAE
#define SET_MEM_ADDR 0x20
#define SET_COL_ADDR 0x21
#define SET_PAGE_ADDR 0x22
#define SET_DISP_START_LINE 0x40
#define SET_SEG_REMAP 0xA0
#define SET_MUX_RATIO 0xA8
#define SET_IREF_SELECT 0xAD
#define SET_COM_OUT_DIR 0xC0
#define SET_DISP_OFFSET 0xD3
#define SET_COM_PIN_CFG 0xDA
#define SET_DISP_CLK_DIV 0xD5
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DESEL 0xDB
#define SET_CHARGE_PUMP 0x8D
#define HEIGHT 64
#define WIDTH 128

// gpio constants
#define CONST_CONSUMER "bonnet"
#define CONST_NUM_BUTTONS 7
// right-side buttons
#define CONST_BUTTON_A 5
#define CONST_BUTTON_B 6
// dpad buttons
#define CONST_BUTTON_L 27
#define CONST_BUTTON_R 23
#define CONST_BUTTON_U 17
#define CONST_BUTTON_D 22
#define CONST_BUTTON_C 4

typedef struct bonnet {

  int i2cfd;
  int i2c_addr;

  struct gpiod_chip *gpio_chip;
  struct gpiod_line_bulk buttons;
  struct gpiod_line_bulk button_events;

  int pages;
  bool powered;

} bonnet;

void bonnet_free(struct bonnet *b) {
  gpiod_line_release_bulk(&(b->buttons));
  gpiod_chip_close(b->gpio_chip);
  close(b->i2cfd);

  free(b);
  b = NULL;
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

  return (0);
}

int bonnet_write_cmd(struct bonnet *b, char cmd) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x00 sets the Co bit to 0, and the D/C# bit to 0
  // to set the next set of bytes as command bytes and
  // not data bytes
  uint8_t cmd_buffer[2] = {0x00, cmd};
  int len_buffer = sizeof(cmd_buffer);
  // printf("[bonnet_write_cmd] sizeof(buffer)= %d\n", len_buffer);
  if (write(b->i2cfd, cmd_buffer, len_buffer) != len_buffer) {
    printf("Error writing command buffer to I2C\n");
    return (-1);
  }
  return (0);
}

int bonnet_write_data(struct bonnet *b, uint8_t data) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x40 sets the Co bit to 0, and the D/C# bit to 1 to set the next set of
  // bytes as data not data bytes
  //
  // if the D/C# bit is set to logic 1, it defines the following data byte as
  // a data which will be stored at the Graphics Display Data RAM
  uint8_t data_buffer[2] = {0x40, data};
  // printf("[bonnet_write_data] data: %x\n", data);
  int len_buffer = sizeof(data_buffer);
  if (write(b->i2cfd, data_buffer, len_buffer) != len_buffer) {
    printf("Error writing data buffer to I2C\n");
    return (-1);
  }
  usleep(1000);
  return (0);
}

int bonnet_write_multi_cmd(struct bonnet *b, uint8_t cmds[], int len_cmds) {
  int write_ret = 0;
  for (int i = 0; i < len_cmds; i++) {
    write_ret = bonnet_write_cmd(b, cmds[i]);
    if (write_ret == -1) {
      return (-1);
    }
  }
  return (0);
}

int bonnet_write_multi_data(struct bonnet *b, uint8_t data[], int count_data) {
  int write_ret = 0;
  for (int i = 0; i < count_data; i++) {
    write_ret = bonnet_write_data(b, data[i]);
    if (write_ret == -1) {
      return (-1);
    }
  }
  return (0);
}

int bonnet_write_multi_data_blast(struct bonnet *b, uint8_t data[],
                                  int len_data) {
  uint8_t *buffer = malloc(sizeof(uint8_t) * len_data + (sizeof(uint8_t)));
  buffer[0] = 0x40;
  memcpy(buffer + 1, data, len_data);
  int ret_val = write(b->i2cfd, buffer, len_data + sizeof(uint8_t));
  free(buffer);
  return ret_val;
}

int bonnet_poweron(struct bonnet *b) {
  return bonnet_write_cmd(b, SET_DISP_ON);
}

int bonnet_write_to_page(struct bonnet *b, uint8_t page, uint8_t start_col,
                         uint8_t end_col, uint8_t data[], int len_data) {
  // set GDDRAM page start address
  bonnet_write_cmd(b, 0xB0 | page);
  bonnet_write_cmd(b, start_col);
  bonnet_write_cmd(b, 0x10 | end_col);

  bonnet_write_multi_data(b, data, len_data);
  return (0);
}

int bonnet_display_initialize(struct bonnet *b) {
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

int main(void) {
  struct bonnet *my_hat = malloc(sizeof(struct bonnet));
  int init_ret = bonnet_struct_init(my_hat, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  bonnet_display_initialize(my_hat);
  uint8_t reset_column_cmds[] = {0x21, 0x00, 127};
  bonnet_write_multi_cmd(my_hat, reset_column_cmds, 3);
  uint8_t buffer[128 * 8];
  int btn_a_pressed;
  int btn_b_pressed;
  int values[CONST_NUM_BUTTONS];
  while (true) {
    gpiod_line_get_value_bulk(&(my_hat->buttons), values);
    btn_a_pressed = values[0];
    btn_b_pressed = values[1];


    // since there is a pull-up, these are default 1
    // a 0 would indicate pressed
    if (btn_a_pressed == 0 && btn_b_pressed == 0) {
      printf("Button A&B pressed, exiting\n");
      break;
    }
    if (btn_a_pressed == 0) {
      memset(&buffer, 0xFF, sizeof(buffer));
      bonnet_write_multi_data_blast(my_hat, buffer, sizeof(buffer));
    }
    if (btn_b_pressed == 0) {
      memset(&buffer, 0x00, sizeof(buffer));
      bonnet_write_multi_data_blast(my_hat, buffer, sizeof(buffer));
    }
    usleep(100);
  }
  memset(&buffer, 0x00, sizeof(buffer));
  bonnet_write_multi_data_blast(my_hat, buffer, sizeof(buffer));
  bonnet_free(my_hat);
  return 0;
}
