#ifndef SEGMENT16_H
#define SEGMENT16_H

#include "../ssd1306_gl.h"
#include "./components.h"

typedef struct uic_segment16_attr {
  ssd1306_fb_vec2_t origin;
  // font height
  uint8_t height;

  // simply use two bytes to determine which of the 16
  // segments should be on
  // each bit of the two bytes is dedicated for each segment
  //     00   01
  //   ----.----
  //   0203040506
  //    | \ | / |
  //     07   08
  //   ----.----
  //   0910111213
  //    | / | \ |
  //   14     15
  //   ----.----
  //
  uint16_t segments;
  bool dot;
  bool color;

  // used to determine how many characters to plot when the `draw` function
  // is called
  //
  int len;
} uic_segment16_attr_t;

void uic_segment16_draw(ssd1306_fb_t *fb, void *_attr);

/**
 * TODO:
 * uic_segment16_free frees all the allocated segment16 memory using the
 * attr->len variable to determine how much memory to free
 *
 */
void uic_segment16_free(uic_t *segment_ptr);

/**
 * uic_segment16_new_from_char takes a single character as the input to create
 *  the 16 segment struct, and `attr` for positioning and sizing for drawing
 *
 */
uic_t *uic_segment16_new_from_char(char c, uic_segment16_attr_t *attr);

/**
 * uic_segment16_new_from_int makes the segment16 object from an integer using
 * the `init_settings` parameter for positioning and sizing for drawing
 *
 * The `spacing` parameter is the spacing between the numbers drawn
 *
 */
uic_t *uic_segment16_new_from_int(int num, uic_segment16_attr_t *init_setting,
                                  int spacing);
/**
 * uic_segment16_new_from_str makes the segment16 object from a string using
 * the `init_settings` parameter for positioning and sizing for drawing
 *
 * The `spacing` parameter is the spacing between the characters drawn
 *
 */
uic_t *uic_segment16_new_from_str(char *str, int str_len,
                                  uic_segment16_attr_t *init_setting,
                                  int spacing);

void uic_segment16__draw_0(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_1(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_2(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_3(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_4(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_5(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_6(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_7(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_8(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_9(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_10(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_11(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_12(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_13(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_14(ssd1306_fb_t *fb, uic_segment16_attr_t attr);
void uic_segment16__draw_15(ssd1306_fb_t *fb, uic_segment16_attr_t attr);

//     00   01
//   ----.----
//   0203040506
//    | \ | / |
//     07   08
//   ----.----
//   0910111213
//    | / | \ |
//   14     15
//   ----.----
static const uint16_t uic_segment16_font[128] = {
    0b1111111111111111, // U+0000 (nul)
    0b0000000000000000, // U+0001
    0b0000000000000000, // U+0002
    0b0000000000000000, // U+0003
    0b0000000000000000, // U+0004
    0b0000000000000000, // U+0005
    0b0000000000000000, // U+0006
    0b0000000000000000, // U+0007
    0b0000000000000000, // U+0008
    0b0000000000000000, // U+0009
    0b0000000000000000, // U+000A
    0b0000000000000000, // U+000B
    0b0000000000000000, // U+000C
    0b0000000000000000, // U+000D
    0b0000000000000000, // U+000E
    0b0000000000000000, // U+000F
    0b0000000000000000, // U+0010
    0b0000000000000000, // U+0011
    0b0000000000000000, // U+0012
    0b0000000000000000, // U+0013
    0b0000000000000000, // U+0014
    0b0000000000000000, // U+0015
    0b0000000000000000, // U+0016
    0b0000000000000000, // U+0017
    0b0000000000000000, // U+0018
    0b0000000000000000, // U+0019
    0b0000000000000000, // U+001A
    0b0000000000000000, // U+001B
    0b0000000000000000, // U+001C
    0b0000000000000000, // U+001D
    0b0000000000000000, // U+001E
    0b0000000000000000, // U+001F
    //     00   01
    //   ----.----
    //   0203040506
    //    | \ | / |
    //     07   08
    //   ----.----
    //   0910111213
    //    | / | \ |
    //   14     15
    //   ----.----
    0b0000000000000000, // U+0020 (space)
    0b0000000000000000, // U+0021 (!)
    0b0000000000000000, // U+0022 (")
    0b0000000000000000, // U+0023 (#)
    0b0000000000000000, // U+0024 ($)
    0b0000000000000000, // U+0025 (%)
    0b0000000000000000, // U+0026 (&)
    0b0000000000010000, // U+0027 (')
    0b0100001000000101, // U+0028 (()
    0b1010000001000010, // U+0029 ())
    0b0000111110111000, // U+002A (*)
    0b0000100110010000, // U+002B (+)
    0b0000000000000000, // U+002C (,)
    0b0000000110000000, // U+002D (-)
    0b0000000000000000, // U+002E (.)
    0b0000010000100000, // U+002F (/)
    0b1110011001100111, // U+0030 (0)
    0b1100100000010001, // U+0031 (1)
    0b1100001111000011, // U+0032 (2)
    0b1110000101000011, // U+0033 (3)
    0b0010000111000100, // U+0034 (4)
    0b1101000110000111, // U+0035 (5)
    0b1110001110000111, // U+0036 (6)
    0b0010000001000011, // U+0037 (7)
    0b1110001111000111, // U+0038 (8)
    0b0010000111000111, // U+0039 (9)
    0b0000000000000000, // U+003A (:)
    0b0000000000000000, // U+003B (;)
    0b0000000000000000, // U+003C (<)
    0b0000000000000000, // U+003D (=)
    0b0000000000000000, // U+003E (>)
    0b0000000000000000, // U+003F (?)
    0b0000000000000000, // U+0040 (@)
    0b0010001111000111, // U+0041 (A)
    0b1110100101010011, // U+0042 (B)
    0b1100001000000111, // U+0043 (C)
    0b1110100001010011, // U+0044 (D)
    0b1100001010000111, // U+0045 (E)
    0b0000001010000111, // U+0046 (F)
    0b1110001100000111, // U+0047 (G)
    0b0010001111000100, // U+0048 (H)
    0b1100100000010011, // U+0049 (I)
    0b1110001001000010, // U+004A (J)
    0b0001001000100100, // U+004B (K)
    0b1100001000000100, // U+004C (L)
    //     00   01
    //   ----.----
    //   0203040506
    //    | \ | / |
    //     07   08
    //   ----.----
    //   0910111213
    //    | / | \ |
    //   14     15
    //   ----.----
    0b0010001001101100, // U+004D (M)
    0b0011001001001100, // U+004E (N)
    0b1110001001000111, // U+004F (O)
    0b0000001111000111, // U+0050 (P)
    0b1111001001000111, // U+0051 (Q)
    0b0001001111000111, // U+0052 (R)
    0b1110000110000111, // U+0053 (S)
    0b0000100000010011, // U+0054 (T)
    0b1110001001000100, // U+0055 (U)
    0b0000011000100100, // U+0056 (V)
    0b0011011001000100, // U+0057 (W)
    0b0001010000101000, // U+0058 (X)
    0b1110000111000100, // U+0059 (Y)
    0b1100010000100011, // U+005A (Z)
    0b1000100000010010, // U+005B ([)
    0b0001000000010000, // U+005C (\)
    0b0100100000010001, // U+005D (])
    0b0000000000000000, // U+005E (^)
    0b0000000000000000, // U+005F (_)
    0b0000000000000000, // U+0060 (`)
    0b0000000000000000, // U+0061 (a)
    0b0000000000000000, // U+0062 (b)
    0b0000000000000000, // U+0063 (c)
    0b0000000000000000, // U+0064 (d)
    0b0000000000000000, // U+0065 (e)
    0b0000000000000000, // U+0066 (f)
    0b0000000000000000, // U+0067 (g)
    0b0000000000000000, // U+0068 (h)
    0b0000000000000000, // U+0069 (i)
    0b0000000000000000, // U+006A (j)
    0b0000000000000000, // U+006B (k)
    0b0000000000000000, // U+006C (l)
    0b0000000000000000, // U+006D (m)
    0b0000000000000000, // U+006E (n)
    0b0000000000000000, // U+006F (o)
    0b0000000000000000, // U+0070 (p)
    0b0000000000000000, // U+0071 (q)
    0b0000000000000000, // U+0072 (r)
    0b0000000000000000, // U+0073 (s)
    0b0000000000000000, // U+0074 (t)
    0b0000000000000000, // U+0075 (u)
    0b0000000000000000, // U+0076 (v)
    0b0000000000000000, // U+0077 (w)
    0b0000000000000000, // U+0078 (x)
    0b0000000000000000, // U+0079 (y)
    0b0000000000000000, // U+007A (z)
    //     00   01
    //   ----.----
    //   0203040506
    //    | \ | / |
    //     07   08
    //   ----.----
    //   0910111213
    //    | / | \ |
    //   14     15
    //   ----.----
    0b0000000000000000, // U+007B ({)
    0b0000000000000000, // U+007C (|)
    0b0000000000000000, // U+007D (})
    0b0000000000000000, // U+007E (~)
    0b0000000000000000  // U+007F
};

#endif
