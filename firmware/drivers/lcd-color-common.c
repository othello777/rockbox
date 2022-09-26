/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 by Dave Chapman
 * Copyright (C) 2009 by Karl Kurbjun
 *
 * Rockbox driver for 16-bit colour LCDs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/


/* to be #included by lcd-16bit*.c */

#if !defined(ROW_INC) || !defined(COL_INC)
#error ROW_INC or COL_INC not defined
#endif

enum fill_opt {
    OPT_NONE = 0,
    OPT_SET,
    OPT_COPY
};

/*** globals ***/
static fb_data lcd_static_framebuffer[LCD_FBHEIGHT][LCD_FBWIDTH]
    IRAM_LCDFRAMEBUFFER CACHEALIGN_AT_LEAST_ATTR(16);

static void *lcd_frameaddress_default(int x, int y);

static fb_data* lcd_backdrop = NULL;
static long lcd_backdrop_offset IDATA_ATTR = 0;

/* shouldn't be changed unless you want system-wide framebuffer changes! */
struct frame_buffer_t lcd_framebuffer_default =
{
    {
        .fb_ptr     = &lcd_static_framebuffer[0][0]
    },
    .get_address_fn = &lcd_frameaddress_default,
    .stride         = STRIDE_MAIN(LCD_WIDTH, LCD_HEIGHT),
    .elems          = (LCD_FBWIDTH*LCD_FBHEIGHT),
};

static struct viewport default_vp =
{
    .x        = 0,
    .y        = 0,
    .width    = LCD_WIDTH,
    .height   = LCD_HEIGHT,
    .flags    = 0,
    .font     = FONT_SYSFIXED,
    .drawmode = DRMODE_SOLID,
    .buffer   = NULL,
    .fg_pattern   = LCD_DEFAULT_FG,
    .bg_pattern   = LCD_DEFAULT_BG,
};

struct viewport* lcd_current_viewport IDATA_ATTR;

static void *lcd_frameaddress_default(int x, int y)
{
    /* the default expects a buffer the same size as the screen */
    struct frame_buffer_t *fb = lcd_current_viewport->buffer;

#if LCD_STRIDEFORMAT == VERTICAL_STRIDE
    size_t element = (x * LCD_NATIVE_STRIDE(fb->stride)) + y;
#else
    size_t element = (y * LCD_NATIVE_STRIDE(fb->stride)) + x;
#endif
    return fb->fb_ptr + element;/*(element % fb->elems);*/
}

/* LCD init */
void lcd_init(void)
{
    /* Initialize the viewport */
    lcd_set_viewport(NULL);

    lcd_clear_display();

    /* Call device specific init */
    lcd_init_device();
    scroll_init();
}

/* Clear the whole display */
void lcd_clear_display(void)
{
    struct viewport* old_vp = lcd_current_viewport;

    lcd_current_viewport = &default_vp;

    lcd_clear_viewport();

    lcd_current_viewport = old_vp;
}

/*** parameter handling ***/

void lcd_set_drawmode(int mode)
{
    lcd_current_viewport->drawmode = mode & (DRMODE_SOLID|DRMODE_INVERSEVID);
}

int lcd_get_drawmode(void)
{
    return lcd_current_viewport->drawmode;
}

void lcd_set_foreground(unsigned color)
{
    lcd_current_viewport->fg_pattern = color;
}

unsigned lcd_get_foreground(void)
{
    return lcd_current_viewport->fg_pattern;
}

void lcd_set_background(unsigned color)
{
    lcd_current_viewport->bg_pattern = color;
}

unsigned lcd_get_background(void)
{
    return lcd_current_viewport->bg_pattern;
}

void lcd_set_drawinfo(int mode, unsigned fg_color, unsigned bg_color)
{
    lcd_set_drawmode(mode);
    lcd_current_viewport->fg_pattern = fg_color;
    lcd_current_viewport->bg_pattern = bg_color;
}

int lcd_getwidth(void)
{
    return lcd_current_viewport->width;
}

int lcd_getheight(void)
{
    return lcd_current_viewport->height;
}

void lcd_setfont(int newfont)
{
    lcd_current_viewport->font = newfont;
}

int lcd_getfont(void)
{
    return lcd_current_viewport->font;
}

int lcd_getstringsize(const unsigned char *str, int *w, int *h)
{
    return font_getstringsize(str, w, h, lcd_current_viewport->font);
}

void lcd_set_backdrop(fb_data* backdrop)
{
    lcd_backdrop = backdrop;
    if (backdrop)
    {
        lcd_backdrop_offset = (intptr_t)backdrop - (intptr_t)FBADDR(0,0);
        lcd_fastpixelfuncs = lcd_fastpixelfuncs_backdrop;
    }
    else
    {
        lcd_backdrop_offset = 0;
        lcd_fastpixelfuncs = lcd_fastpixelfuncs_bgcolor;
    }
}

fb_data* lcd_get_backdrop(void)
{
    return lcd_backdrop;
}

/* Draw a full native bitmap */
void lcd_bitmap(const fb_data *src, int x, int y, int width, int height)
{
    lcd_bitmap_part(src, 0, 0, STRIDE(SCREEN_MAIN, width, height), x, y, width, height);
}

/* Draw a full native bitmap with a transparent color */
void lcd_bitmap_transparent(const fb_data *src, int x, int y,
                            int width, int height)
{
    lcd_bitmap_transparent_part(src, 0, 0,
                                STRIDE(SCREEN_MAIN, width, height), x, y, width, height);
}

/* draw alpha bitmap for anti-alias font */
void ICODE_ATTR lcd_alpha_bitmap_part(const unsigned char *src, int src_x,
                                      int src_y, int stride, int x, int y,
                                      int width, int height)
{
    lcd_alpha_bitmap_part_mix(NULL, src, src_x, src_y, x, y, width, height, 0, stride);
}

/* Draw a partial bitmap (mono or native) including alpha channel */
void ICODE_ATTR lcd_bmp_part(const struct bitmap* bm, int src_x, int src_y,
                                int x, int y, int width, int height)
{
    int bitmap_stride = LCD_FBSTRIDE(bm->width, bm->height);
    if (bm->format == FORMAT_MONO)
        lcd_mono_bitmap_part(bm->data, src_x, src_y, bm->width, x, y, width, height);
    else if (bm->alpha_offset > 0)
        lcd_alpha_bitmap_part_mix((fb_data*)bm->data, bm->data+bm->alpha_offset,
            src_x, src_y, x, y, width, height,
            bitmap_stride, ALIGN_UP(bm->width, 2));
    else
        lcd_bitmap_transparent_part((fb_data*)bm->data,
            src_x, src_y, bitmap_stride, x, y, width, height);
}

/* Draw a native bitmap with alpha channel */
void ICODE_ATTR lcd_bmp(const struct bitmap *bmp, int x, int y)
{
    lcd_bmp_part(bmp, 0, 0, x, y, bmp->width, bmp->height);
}

/**
 * |R|   |1.000000 -0.000001  1.402000| |Y'|
 * |G| = |1.000000 -0.334136 -0.714136| |Pb|
 * |B|   |1.000000  1.772000  0.000000| |Pr|
 * Scaled, normalized, rounded and tweaked to yield RGB 565:
 * |R|   |74   0 101| |Y' -  16| >> 9
 * |G| = |74 -24 -51| |Cb - 128| >> 8
 * |B|   |74 128   0| |Cr - 128| >> 9
 */
#define YFAC    (74)
#define RVFAC   (101)
#define GUFAC   (-24)
#define GVFAC   (-51)
#define BUFAC   (128)

static inline int clamp(int val, int min, int max)
{
    if (val < min)
        val = min;
    else if (val > max)
        val = max;
    return val;
}

/* Fill a rectangle with a gradient. This function draws only the partial
 * gradient. It assumes the original gradient is src_height high and skips
 * the first few rows. This is useful for drawing only the bottom half of
 * a full gradient.
 *
 * height == src_height and row_skip == 0 will draw the full gradient
 *
 * x, y, width, height - dimensions describing the rectangle
 * start_rgb - beginning color of the gradient
 * end_rgb - end color of the gradient
 * src_height - assumed original height (only height rows will be drawn)
 * row_skip - how many rows of the original gradient to skip
 */
void lcd_gradient_fillrect_part(int x, int y, int width, int height,
        unsigned start_rgb, unsigned end_rgb, int src_height, int row_skip)
{
    int old_pattern = lcd_current_viewport->fg_pattern;
    int step_mul, i;
    int x1, x2;
    x1 = x;
    x2 = x + width;

    if (height <= 0) return;

    step_mul = (1 << 16) / src_height;
    int h_r = RGB_UNPACK_RED(start_rgb);
    int h_g = RGB_UNPACK_GREEN(start_rgb);
    int h_b = RGB_UNPACK_BLUE(start_rgb);
    int rstep = (h_r - RGB_UNPACK_RED(end_rgb)) * step_mul;
    int gstep = (h_g - RGB_UNPACK_GREEN(end_rgb)) * step_mul;
    int bstep = (h_b - RGB_UNPACK_BLUE(end_rgb)) * step_mul;
    h_r = (h_r << 16) + (1 << 15);
    h_g = (h_g << 16) + (1 << 15);
    h_b = (h_b << 16) + (1 << 15);

    if (row_skip > 0)
    {
        h_r -= rstep * row_skip;
        h_g -= gstep * row_skip;
        h_b -= bstep * row_skip;
    }

    for(i = y; i < y + height; i++) {
        lcd_current_viewport->fg_pattern = LCD_RGBPACK(h_r >> 16, h_g >> 16, h_b >> 16);
        lcd_hline(x1, x2, i);
        h_r -= rstep;
        h_g -= gstep;
        h_b -= bstep;
    }

    lcd_current_viewport->fg_pattern = old_pattern;
}

/* Fill a rectangle with a gradient. The gradient's color will fade from
 * start_rgb to end_rgb over the height of the rectangle
 *
 * x, y, width, height - dimensions describing the rectangle
 * start_rgb - beginning color of the gradient
 * end_rgb - end color of the gradient
 */
void lcd_gradient_fillrect(int x, int y, int width, int height,
        unsigned start_rgb, unsigned end_rgb)
{
    lcd_gradient_fillrect_part(x, y, width, height, start_rgb, end_rgb, height, 0);
}
