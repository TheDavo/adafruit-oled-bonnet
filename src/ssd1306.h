#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

// display driver constants
#define ssd1306_SET_CONTRAST 0x81
#define ssd1306_SET_ENTIRE_ON 0xA5
#define ssd1306_SET_ENTIRE_RAM 0xA4
#define ssd1306_SET_NORM_INV 0xA6
#define ssd1306_SET_DISP_ON 0xAF
#define ssd1306_SET_DISP_OFF 0xAE
#define ssd1306_SET_MEM_ADDR 0x20
#define ssd1306_SET_COL_ADDR 0x21
#define ssd1306_SET_PAGE_ADDR 0x22
#define ssd1306_SET_DISP_START_LINE 0x40
#define ssd1306_SET_SEG_REMAP 0xA0
#define ssd1306_SET_MUX_RATIO 0xA8
#define ssd1306_SET_IREF_SELECT 0xAD
#define ssd1306_SET_COM_OUT_DIR 0xC0
#define ssd1306_SET_DISP_OFFSET 0xD3
#define ssd1306_SET_COM_PIN_CFG 0xDA
#define ssd1306_SET_DISP_CLK_DIV 0xD5
#define ssd1306_SET_PRECHARGE 0xD9
#define ssd1306_SET_VCOM_DESEL 0xDB
#define ssd1306_SET_CHARGE_PUMP 0x8D
#define ssd1306_HEIGHT 64
#define ssd1306_WIDTH 128
#define ssd1306_PAGES 8
#define ssd1306_PAGE_HEIGHT 8
#define FRAMEBUFFER_SIZE ((HEIGHT * WIDTH) / (PAGE_HEIGHT))

typedef struct {
  int i2cfd;
  int i2c_addr;
  // int reset_gpio;
  // uint8_t height;
  // uint8_t width;
} ssd1306_t;

/**
 * ssd1306_struct_innit initializes the struct and opens the i2c file
 * descriptor for communication
 *
 * TODO: implement driver for various display sizes
 *
 * Returns (-1) on error or (0) on success
 */
int ssd1306_struct_init(ssd1306_t *ssd, int addr, char *dev_i2c);

/**
 * ssd1306_free closes the file descriptor to the I2C channel
 */
void ssd1306_free(ssd1306_t ssd);

/**
 * ssd1306_write_cmd takes a I2C command and writes it to the display driver
 *
 * Returns the return value from the write() command, which is
 * the number of bytes written, or (-1) on error
 *
 */
int ssd1306_write_cmd(ssd1306_t ssd, uint8_t cmd);

/**
 * ssd1306_write_cmd_multi writes all the commands in *cmds to the display
 * driver.
 * NOTE: The cmd_sizeof is not the count of elements in *cmds,  but the
 * value of sizeof(cmds)
 *
 * Returns the return value from the write() command, which is
 * the number of bytes written, or (-1) on error
 */
int ssd1306_write_cmd_multi(ssd1306_t ssd, uint8_t *cmds, int cmd_sizeof);

// GDDRAM-related functions
/**
 * ssd1306_write_data writes a data byte to the display driver
 *
 * Data written to the device is placed into the graphics display RAM of the
 * display driver
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int ssd1306_write_data(ssd1306_t ssd, uint8_t data);

/**
 * ssd1306_write_multi_data writes the data in *data to the display driver,
 * placing the buffer into the display driver's display RAM
 * NOTE: The data_sizeof is not the count of elements in *data, but the
 * value of sizeof(data)
 *
 * Returns (-1) on failure and (0) on success
 *
 */
int ssd1306_write_data_multi(ssd1306_t ssd, uint8_t *data, int data_sizeof);

/**
 * ssd1306_write_data_to_segment changes the start and end page pointers of
 * the display's current setting to write to only a particular segment
 */
int ssd1306_write_data_to_segment(ssd1306_t ssd, uint8_t page, uint8_t segment,
                                  uint8_t data);
#endif
