/*******************************************************************************
 * Size: 10 px
 * Bpp: 1
 * Opts: --bpp 1 --size 10 --font F:/Programming/SquareLine/GPS_TFT/assets/fonts/digital-7 (mono).ttf -o F:/Programming/SquareLine/GPS_TFT/assets/fonts\ui_font_Digital_Mono_10.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_DIGITAL_MONO_10
#define UI_FONT_DIGITAL_MONO_10 1
#endif

#if UI_FONT_DIGITAL_MONO_10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xee,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0x4b, 0xb4,

    /* U+0024 "$" */
    0x2f, 0xaa, 0xf3, 0x3f, 0x20,

    /* U+0025 "%" */
    0xd7, 0x88, 0x4, 0x3e, 0xe0,

    /* U+0026 "&" */
    0xf9, 0x9f, 0x99, 0xf0,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x72, 0x9, 0x18,

    /* U+0029 ")" */
    0xc4, 0x82, 0x70,

    /* U+002A "*" */
    0x7b, 0x80,

    /* U+002B "+" */
    0x5d, 0x0,

    /* U+002C "," */
    0xc0,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x12, 0x20, 0x44, 0x80,

    /* U+0030 "0" */
    0xf9, 0x90, 0x99, 0xf0,

    /* U+0031 "1" */
    0xee,

    /* U+0032 "2" */
    0xf1, 0x1f, 0x88, 0xe0,

    /* U+0033 "3" */
    0xf1, 0x1f, 0x11, 0xf0,

    /* U+0034 "4" */
    0x99, 0x9f, 0x11, 0x10,

    /* U+0035 "5" */
    0xe8, 0x8f, 0x11, 0xf0,

    /* U+0036 "6" */
    0xe8, 0x8f, 0x99, 0xf0,

    /* U+0037 "7" */
    0xf9, 0x90, 0x11, 0x10,

    /* U+0038 "8" */
    0xf9, 0x9f, 0x99, 0xf0,

    /* U+0039 "9" */
    0xf9, 0x9f, 0x11, 0xf0,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x86,

    /* U+003C "<" */
    0x69,

    /* U+003D "=" */
    0xfc,

    /* U+003E ">" */
    0x96,

    /* U+003F "?" */
    0xf1, 0x1f, 0x80, 0x80,

    /* U+0040 "@" */
    0xfb, 0xd0, 0xdb, 0xe0,

    /* U+0041 "A" */
    0xf9, 0x9f, 0x99, 0x90,

    /* U+0042 "B" */
    0xe9, 0x9f, 0x99, 0xe0,

    /* U+0043 "C" */
    0xf8, 0x80, 0x88, 0xf0,

    /* U+0044 "D" */
    0xe9, 0x90, 0x99, 0xe0,

    /* U+0045 "E" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0046 "F" */
    0xf8, 0x8f, 0x88, 0x80,

    /* U+0047 "G" */
    0xe8, 0x83, 0x99, 0xf0,

    /* U+0048 "H" */
    0x99, 0x9f, 0x99, 0x90,

    /* U+0049 "I" */
    0xee,

    /* U+004A "J" */
    0x11, 0x10, 0x99, 0xf0,

    /* U+004B "K" */
    0x9a, 0xcf, 0x99, 0x90,

    /* U+004C "L" */
    0x88, 0x80, 0x88, 0xf0,

    /* U+004D "M" */
    0xff, 0xab, 0x68,

    /* U+004E "N" */
    0x9d, 0xb0, 0x99, 0x90,

    /* U+004F "O" */
    0x69, 0x90, 0x99, 0x60,

    /* U+0050 "P" */
    0xe9, 0x9f, 0x88, 0x80,

    /* U+0051 "Q" */
    0xf9, 0x90, 0x9b, 0xf0,

    /* U+0052 "R" */
    0xf9, 0x9f, 0xca, 0x90,

    /* U+0053 "S" */
    0xf8, 0x8f, 0x11, 0xf0,

    /* U+0054 "T" */
    0x71, 0x8, 0x2, 0x10, 0x80,

    /* U+0055 "U" */
    0x99, 0x90, 0x99, 0xf0,

    /* U+0056 "V" */
    0x99, 0x90, 0x96, 0x0,

    /* U+0057 "W" */
    0xb6, 0xaf, 0xf8,

    /* U+0058 "X" */
    0x96, 0x60, 0x66, 0x90,

    /* U+0059 "Y" */
    0x99, 0x9f, 0x11, 0xf0,

    /* U+005A "Z" */
    0xf2, 0x20, 0x48, 0xe0,

    /* U+005B "[" */
    0xe8, 0xac,

    /* U+005C "\\" */
    0x84, 0x40, 0x22, 0x10,

    /* U+005D "]" */
    0xd4, 0x5c,

    /* U+005E "^" */
    0x69, 0x90,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x90,

    /* U+0061 "a" */
    0xf9, 0x9f, 0x99, 0x90,

    /* U+0062 "b" */
    0xe9, 0x9f, 0x99, 0xe0,

    /* U+0063 "c" */
    0xf8, 0x80, 0x88, 0xf0,

    /* U+0064 "d" */
    0xe9, 0x90, 0x99, 0xe0,

    /* U+0065 "e" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0066 "f" */
    0xf8, 0x8f, 0x88, 0x80,

    /* U+0067 "g" */
    0xe8, 0x83, 0x99, 0xf0,

    /* U+0068 "h" */
    0x99, 0x9f, 0x99, 0x90,

    /* U+0069 "i" */
    0xee,

    /* U+006A "j" */
    0x11, 0x10, 0x99, 0xf0,

    /* U+006B "k" */
    0x9a, 0xcf, 0x99, 0x90,

    /* U+006C "l" */
    0x88, 0x80, 0x88, 0xf0,

    /* U+006D "m" */
    0xff, 0xab, 0x68,

    /* U+006E "n" */
    0x9d, 0xb0, 0x99, 0x90,

    /* U+006F "o" */
    0x69, 0x90, 0x99, 0x60,

    /* U+0070 "p" */
    0xe9, 0x9f, 0x88, 0x80,

    /* U+0071 "q" */
    0xf9, 0x90, 0x9b, 0xf0,

    /* U+0072 "r" */
    0xf9, 0x9f, 0xca, 0x90,

    /* U+0073 "s" */
    0xf8, 0x8f, 0x11, 0xf0,

    /* U+0074 "t" */
    0x71, 0x8, 0x2, 0x10, 0x80,

    /* U+0075 "u" */
    0x99, 0x90, 0x99, 0xf0,

    /* U+0076 "v" */
    0x99, 0x90, 0x96, 0x0,

    /* U+0077 "w" */
    0xb6, 0xaf, 0xf8,

    /* U+0078 "x" */
    0x96, 0x60, 0x66, 0x90,

    /* U+0079 "y" */
    0x99, 0x9f, 0x11, 0xf0,

    /* U+007A "z" */
    0xf2, 0x20, 0x48, 0xe0,

    /* U+007B "{" */
    0x69, 0x44, 0x98,

    /* U+007C "|" */
    0xfe,

    /* U+007D "}" */
    0xc9, 0x14, 0xb0,

    /* U+007E "~" */
    0xf3
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 73, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 73, .box_w = 1, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 73, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 3, .adv_w = 73, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 5, .adv_w = 73, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 10, .adv_w = 73, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 73, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 19, .adv_w = 73, .box_w = 1, .box_h = 2, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 20, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 73, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 28, .adv_w = 73, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 30, .adv_w = 73, .box_w = 1, .box_h = 2, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 31, .adv_w = 73, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 32, .adv_w = 73, .box_w = 1, .box_h = 1, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 41, .adv_w = 73, .box_w = 1, .box_h = 7, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 50, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 54, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 74, .adv_w = 73, .box_w = 1, .box_h = 4, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 75, .adv_w = 73, .box_w = 1, .box_h = 7, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 76, .adv_w = 73, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 77, .adv_w = 73, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 78, .adv_w = 73, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 79, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 83, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 87, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 99, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 103, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 73, .box_w = 1, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 124, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 159, .adv_w = 73, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 73, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 73, .box_w = 2, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 73, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 197, .adv_w = 73, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 73, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 199, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 227, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 73, .box_w = 1, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 232, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 244, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 267, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 271, .adv_w = 73, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 276, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 284, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 73, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 73, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 73, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Digital_Mono_10 = {
#else
lv_font_t ui_font_Digital_Mono_10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 9,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_DIGITAL_MONO_10*/

