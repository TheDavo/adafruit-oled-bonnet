#ifndef BONNET_H
#define BONNET_H

#include "ssd1306.h"
#include <gpiod.h>
#include <stdint.h>

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

#define BONNET_BUTTON_IDX_A 0
#define BONNET_BUTTON_IDX_B 1
#define BONNET_BUTTON_IDX_CENTER 2
#define BONNET_BUTTON_IDX_DOWN 3
#define BONNET_BUTTON_IDX_LEFT 4
#define BONNET_BUTTON_IDX_RIGHT 5
#define BONNET_BUTTON_IDX_UP 6

typedef struct bonnet {
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
 *  This function will also free `b` and set it to NULL
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
                            int lenbytes_data);

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

void bonnet_action_write_to_segment(struct bonnet *b, uint8_t page, uint8_t col,
                                    uint8_t data);

/**
 * bonnet_action_clear_display clears the framebuffer and updates the display.
 * This function _will_ overwrite the internal framebuffer and set it all
 * to zero.
 *
 * This is an action function and will update the rendering on the display.
 */
void bonnet_action_clear_display(struct bonnet *b);

void bonnet_display_framebuffer(struct bonnet b);

// GPIO

typedef enum {
  bonnet_e_button_state_down = 0,
  bonnet_e_button_state_up = 1,
} bonnet_e_button_state;

/**
 * bonnet_e_button is a wrapper on the button macro values, and represents
 * the indexes of the buttons as an enum
 *
 */
typedef enum {
  bonnet_e_button_a = BONNET_BUTTON_IDX_A,
  bonnet_e_button_b = BONNET_BUTTON_IDX_B,
  bonnet_e_button_center = BONNET_BUTTON_IDX_CENTER,
  bonnet_e_button_down = BONNET_BUTTON_IDX_DOWN,
  bonnet_e_button_left = BONNET_BUTTON_IDX_LEFT,
  bonnet_e_button_right = BONNET_BUTTON_IDX_RIGHT,
  bonnet_e_button_up = BONNET_BUTTON_IDX_UP,
} bonnet_e_button;

/**
 * bonnet_get_button_state calls the appropriate `gpiod` function to get the
 * state of `button`, and returns the button's state: up or down, as an enum
 * value
 *
 */
bonnet_e_button_state bonnet_button_get_state(struct bonnet b,
                                              bonnet_e_button button);

/**
 * bonnet_get_button_states updates the `*states` array with the states of all
 * the buttons available on this bonnet
 *
 * The `states` array MUST be of size seven (7) to update all GPIO buttons
 * correctly. Array size less than seven (7) will cause memory issues
 *
 * The array will be populated with the states in the following order:
 * states[0] --> Button A
 *
 * states[1] --> Button B
 *
 * states[2] --> Button Center
 *
 * states[3] --> Button Down
 *
 * states[4] --> Button Left
 *
 * states[5] --> Button Right
 *
 * states[6] --> Button Up
 *
 */
void bonnet_button_get_states(struct bonnet b, bonnet_e_button_state *states);

/**
 * bonnet_button_name_into_str takes a button and returns the button's name
 * as a string
 *
 * This function is useful for debug mainly
 *
 */
char *bonnet_button_name_into_str(bonnet_e_button button);

/**
 * bonnet_button_state_into_str takes a button state and returns it as a string
 *
 * This function is useful for debug mainly
 *
 */
char *bonnet_button_state_into_str(bonnet_e_button_state state);

// Wrapper Functions

/**
 * bonnet_set_display_off turns the display off
 * This is a wrapper functions that writes the correct SSD1306 command
 * to turn off the display
 */
void bonnet_set_display_off(struct bonnet b);

#endif
