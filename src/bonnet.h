#ifndef BONNET_H
#define BONNET_H

#include <gpiod.h>
#include <stdint.h>
#include "ssd1306.h"

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
#define PAGES 8
#define PAGE_HEIGHT 8
#define FRAMEBUFFER_SIZE ((HEIGHT * WIDTH) / (PAGE_HEIGHT))

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

  ssd1306_t ssd;
  struct gpiod_chip *gpio_chip;
  struct gpiod_line_bulk buttons;
  struct gpiod_line_bulk button_events;

  uint8_t framebuffer[FRAMEBUFFER_SIZE];

} bonnet;

/**
 * bonnet_struct_init opens the I2C file descriptor and opens the GPIO chip
 * and calls bonnet__init_gpio_lines to request the button lines
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int bonnet_struct_init(struct bonnet *b, uint8_t bonnet_i2c_addr);

/**
 *  bonnet_close is the destructor to the bonnet, it handles releasing
 *  the gpio lines and chip, and closing the I2C file descriptor.
 *
 *  This function will also free @param b and set it to NULL
 *
 * Returns (-1) on failure and (0) on success
 *
 */
void bonnet_close(struct bonnet *b);

/**
 * bonnet__init_gpio_lines initializes the GPIO lines for the buttons
 * it is meant to be used after the bonnet_struct_init() function is called
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int bonnet__init_gpio_lines(struct bonnet *b);

// Display Functions

/**
 * bonnet_display_initialize sends commands to the display driver for a proper
 * wakeup, setting up values such as mux ratio, display offset,
 * charge pump on/off, etc.
 *
 * Returns:
 *   (-1) on error or (0) on success
 *
 * WARNING:
 * This initialization function is built for the Adafruit OLED 128x64 bonnet
 * and will not work with other devices
 */
int bonnet_display_initialize(struct bonnet b);

/**
 * bonnet_write_cmd takes a I2C command and writes it to the display driver
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int bonnet_write_cmd(const struct bonnet b, uint8_t cmd);

/**
 * bonnet_write_multi_cmd loops over cmds[] and writes to the display driver
 * using bonnet_write_cmd().
 *
 * NOTE:
 * This could be improved to be like bonnet_write_multi_data in which one
 * write command is used instead of multiple invocations
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int bonnet_write_multi_cmd(const struct bonnet b, uint8_t cmds[], int len_cmds);

/**
 * bonnet_write_data writes a data byte to the display driver
 *
 * Data written to the device is placed into the graphics display RAM of the
 * display driver
 *
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int bonnet_write_data(const struct bonnet b, uint8_t data);

/**
 * bonnet_write_multi_data writes the data[] to the display driver, placing
 * the buffer into the display driver's display RAM
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int bonnet_write_multi_data(const struct bonnet b, uint8_t data[],
                            int count_data);

// Drawing functions

/**
 * bonnet__get_framebuffer_data_at returns the byte that the pixel (x,y) would
 * be at
 *
 * Bonnet's framebuffer is segmented with pages and not individual pixels, and
 * the framebuffer mimics that structure
 *
 * This function is correctly used by bonnet_action_write_to_pixel and should
 * not be called on its own.
 *
 */
uint8_t bonnet__get_framebuffer_data_at(struct bonnet b, uint8_t x, uint8_t y);

/**
 * bonnet__set_framebuffer_data_at sets the byte value at the page and column
 * determined by where a pixel at (x,y) would be at
 *
 * This function is correctly used by bonnet_action_write_to_pixel and should
 * not be called on its own.
 */
void bonnet__set_framebuffer_data_at(struct bonnet *b, uint8_t x, uint8_t y,
                                     uint8_t value);
/**
 * bonnet_action_write_to_pixel updates the pixel at (x, y) on if `set` is true
 * or off if `set` is false
 *
 * This is an action function and will update the rendering on the display.
 */
void bonnet_action_write_to_pixel(struct bonnet *b, uint8_t x, uint8_t y,
                                  bool set);

void bonnet_action_write_to_column(struct bonnet *b, uint8_t page, uint8_t col,
                                  uint8_t new_data);

/**
 * bonnet_action_clear_display clears the framebuffer and updates the display.
 * This function _will_ overwrite the internal framebuffer and set it all
 * to zero.
 *
 * This is an action function and will update the rendering on the display.
 */
void bonnet_action_clear_display(struct bonnet *b);

void bonnet_display_framebuffer(struct bonnet b);

#endif
