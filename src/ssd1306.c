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
#include <sys/types.h>
#include <unistd.h>

#include "ssd1306.h"
#include "ssd1306_gl.h"
int ssd1306_struct_init(ssd1306_t *ssd, uint8_t addr, char *dev_i2c,
                        ssd1306_display_size_e display_size) {
  if (dev_i2c == NULL || ssd == NULL) {
    return (-1);
  }

  ssd->i2c_addr = addr;
  printf("[SSD1306]: Opening i2c\n");
  if ((ssd->i2cfd = open(dev_i2c, O_RDWR)) < 0) {
    printf("Error opening i2c file descriptor\n");
    return (-1);
  }
  printf("[SSD1306]: File descriptor %d opened\n", ssd->i2cfd);

  printf("[SSD1306]: Setting up i2c\n");
  if (ioctl(ssd->i2cfd, I2C_SLAVE, ssd->i2c_addr)) {
    printf("[SSD1306]: Error setting I2C slave addr\n");
    close(ssd->i2cfd);
    return (-1);
  }

  ssd1306_fb_t *fb = ssd1306_fb_new(display_size, false);
  if (!fb) {
    return (-1);
  }
  ssd->framebuf = fb;
  return (0);
}

void ssd1306_free(ssd1306_t ssd) {
  close(ssd.i2cfd);
  ssd1306_fb_free(ssd.framebuf);
}

int ssd1306_write_cmd(ssd1306_t ssd, uint8_t cmd) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x80 sets the Co bit to 1, and the D/C# bit to 0
  // which can send just one command byte
  uint8_t buffer[2] = {0x80, cmd};
  int len_buffer = sizeof(buffer);
  int bytes_written = write(ssd.i2cfd, buffer, len_buffer);
  return bytes_written;
}

int ssd1306_write_cmd_multi(ssd1306_t ssd, uint8_t *cmds, int cmd_sizeof) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x00 sets the Co bit to 0, and the D/C# bit to 0
  // which can send continuous command bytes

  int buffer_size = cmd_sizeof + sizeof(uint8_t);
  uint8_t *buffer = malloc(buffer_size);
  if (buffer == NULL) {
    printf("[SSD1306]: Failed to malloc in function ssd1306_write_cmd_multi\n");
    return (-1);
  }

  buffer[0] = 0x00;
  memcpy(buffer + sizeof(uint8_t), cmds, cmd_sizeof);
  int bytes_written = write(ssd.i2cfd, buffer, buffer_size);

  free(buffer);
  return bytes_written;
}

int ssd1306_write_data(ssd1306_t ssd, uint8_t data) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0xC0 sets the Co bit to 1, and the D/C# bit to 1 to set the next set of
  // bytes as data, the Co bit as 1 allows only one byte
  // before the I2C stop is expected
  //
  // if the D/C# bit is set to logic 1, it defines the following data byte as
  // a data which will be stored at the Graphics Display Data RAM
  uint8_t buffer[2] = {0xC0, data};
  int bytes_written = write(ssd.i2cfd, buffer, sizeof(buffer));

  return bytes_written;
}

int ssd1306_write_data_multi(ssd1306_t ssd, uint8_t *data, int data_sizeof) {

  int buffer_size = data_sizeof + sizeof(uint8_t);
  uint8_t *buffer = malloc(buffer_size);
  buffer[0] = 0x40;
  memcpy(buffer + 1, data, data_sizeof);
  int bytes_written = write(ssd.i2cfd, buffer, buffer_size);
  free(buffer);
  return bytes_written;
}

int ssd1306_write_framebuffer_all(ssd1306_t ssd) {

  // uint8_t data_start[1] = {0x40};
  // write(ssd.i2cfd, data_start, sizeof(data_start));
  // int bytes_written = write(ssd.i2cfd, ssd.framebuf->framebuf,
  //                           (ssd.framebuf->height * ssd.framebuf->width) / 8);
  // return bytes_written;
  return ssd1306_write_data_multi(ssd, ssd.framebuf->framebuf, 1024);
}
int ssd1306_write_data_to_segment(ssd1306_t ssd, uint8_t page, uint8_t segment,
                                  uint8_t data) {

  uint8_t cmds[] = {ssd1306_SET_COL_ADDR,  segment, segment,
                    ssd1306_SET_PAGE_ADDR, page,    page};
  ssd1306_write_cmd_multi(ssd, cmds, sizeof(cmds));
  int bytes_written = ssd1306_write_data(ssd, data);
  return bytes_written;
}
