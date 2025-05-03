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

#include "ssd1306.h"
int ssd1306_struct_init(ssd1306_t *ssd, int addr, char *dev_i2c) {
  if (dev_i2c == NULL || ssd == NULL) {
    return (-1);
  }

  ssd->i2c_addr = addr;
  printf("[SSD1306]: Opening i2c\n");
  if ((ssd->i2cfd = open(dev_i2c, O_RDWR)) < 0) {
    printf("Error opening i2c file descriptor\n");
    return (-1);
  }

  printf("[SSD1306]: Setting up i2c\n");
  if (ioctl(ssd->i2cfd, I2C_SLAVE, ssd->i2c_addr)) {
    printf("[SSD1306]: Error setting I2C slave addr\n");
    close(ssd->i2cfd);
    return (-1);
  }

  return (0);
}

void ssd1306_free(ssd1306_t ssd) { close(ssd.i2cfd); }

int ssd1306_write_cmd(ssd1306_t ssd, uint8_t cmd) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x80 sets the Co bit to 1, and the D/C# bit to 0
  // which can send just one command byte
  uint8_t buffer[2] = {0x80, cmd};
  int len_buffer = sizeof(len_buffer);
  int bytes_written = write(ssd.i2cfd, buffer, len_buffer);
  return bytes_written;
}

int ssd1306_write_cmd_multi(ssd1306_t ssd, uint8_t *cmds, int cmd_sizeof) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x00 sets the Co bit to 0, and the D/C# bit to 0
  // which can send continuous command bytes

  uint8_t *buffer = malloc(cmd_sizeof + sizeof(uint8_t));
  if (buffer == NULL) {
    printf("[SSD1306]: Failed to malloc in function ssd1306_write_cmd_multi\n");
    return (-1);
  }

  buffer[0] = 0x00;
  memcpy(buffer + 1, cmds, cmd_sizeof);

  int bytes_written =
      write(ssd.i2cfd, buffer, sizeof(buffer)) != sizeof(buffer);

  free(buffer);
  return bytes_written;
}

int ssd1306_write_data(ssd1306_t ssd, uint8_t data) {
  // Section 8.1.5.2 of SSD1306 datasheet
  // 0x90 sets the Co bit to 1, and the D/C# bit to 1 to set the next set of
  // bytes as data not data bytes, the Co bit as 1 allows only one byte
  // before the I2C stop is expected
  //
  // if the D/C# bit is set to logic 1, it defines the following data byte as
  // a data which will be stored at the Graphics Display Data RAM
  uint8_t buffer[2] = {0x90, data};
  int len_buffer = sizeof(buffer);
  int bytes_written =
      write(ssd.i2cfd, buffer, sizeof(buffer)) != sizeof(buffer);

  return bytes_written;
}

int ssd1306_write_data_multi(ssd1306_t ssd, uint8_t *data, int data_sizeof) {

  uint8_t *buffer = malloc(data_sizeof + (sizeof(uint8_t)));
  buffer[0] = 0x40;
  memcpy(buffer + 1, data, data_sizeof);
  int ret_val = write(ssd.i2cfd, buffer, sizeof(buffer));
  free(buffer);
  return ret_val;
}
