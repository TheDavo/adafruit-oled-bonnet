# Introduction

This library is a high-level driver written for the
[Adafruit Bonnet Link](https://www.adafruit.com/product/3531) which features
a 128 x 64 display using the SSD1306 display driver, a d-pad, and two buttons.

The library is written on a Raspberry Pi 4, using a few system dependencies,
mainly using Linux `write` and `ioctl` syscalls to achieve the I2C calls.

The main dependency for this library is `libgpiod`, which allows for easier
control, reading, and writing of the d-pad and two buttons, which are simply
hooked up to GPIO pins on the Raspberry Pi.

## Setting Up the Bonnet and Display

To set up the bonnet, and the display, create a `bonnet` struct and call its
initialization function, which handles the creation of the internal SSD1306
struct allocation and initialization.

```c
#include "../src/bonnet.h"
#include "../src/ssd1306.h"
#include "../src/ssd1306_gl.h"

#include <stdio.h>

int main(void) {
  struct bonnet bonnet;
  int init_ret = bonnet_struct_init(&bonnet, 0x3C);
  printf("bonnet_init ret %d\n", init_ret);
  if (init_ret != 0) {
    printf("Error initializing Bonnet, exiting\n");
    return -1;
  }
  // sends the specific commands to initialize the SSD1306 display driver
  // for this specific Raspberry Pi hat
  bonnet_display_initialize(bonnet);

  bonnet_action_clear_display(&bonnet);

  // rest of main

}
```

After the initialization, the `ssd1306` library calls can be used to update
the internal framebuffer property of the `ssd1306` struct. The `ssd1306` library
also contains the constants/defines to the registers of the device, so that the
properties of the display can be changed. Things such as the display contrast,
scrolling, pixel map orientation, etc..

Calls to the `ssd1306_fb` library is used to properly update the buffer. Do not
directly update the framebuffer without calling the `ssd1306_fb` API. This is
error prone and can cause memory issues.

# Simple Graphics API

The `sdd1306_fb` library can draw the basic shapes:

| Drawing Type | Function to Call |
| :----------- | :--------------- |
| Pixel | `ssd1306_fb_draw_pixel` |
| Line, with Cartesian Coordinates | `ssd1306_fb_draw_line_carte` |
| Line, with Polar Coordinates | `ssd1306_fb_draw_line_polar` |
| Rectangle | `ssd1306_fb_draw_rect` |
| Circle | `ssd1306_fb_draw_circle` |
| Arc | `ssd1306_fb_draw_arc` |
