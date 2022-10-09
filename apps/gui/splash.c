/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) Daniel Stenberg (2002)
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
#include "stdarg.h"
#include "string.h"
#include "rbunicode.h"
#include "stdio.h"
#include "kernel.h"
#include "screen_access.h"
#include "lang.h"
#include "settings.h"
#include "talk.h"
#include "splash.h"
#include "viewport.h"
#include "strtok_r.h"
#include "scrollbar.h"

#define MAXLINES  (LCD_HEIGHT/6)
#define MAXBUFFER 512
#define RECT_SPACING 2
#define SPLASH_MEMORY_INTERVAL (HZ)

static bool splash_internal(struct screen * screen, const char *fmt, va_list ap,
                            struct viewport *vp, int addl_lines)
{
    char splash_buf[MAXBUFFER];
    char *lines[MAXLINES];
    char *next;
    char *lastbreak = NULL;
    char *store = NULL;
    int line = 0;
    int x = 0;
    int y, i;
    int space_w, w, h;
    int width, height;
    int maxw = 0;

    screen->getstringsize(" ", &space_w, &h);
    y = h + (addl_lines * h);

    vsnprintf(splash_buf, sizeof(splash_buf), fmt, ap);
    va_end(ap);

    /* break splash string into display lines, doing proper word wrap */

    next = strtok_r(splash_buf, " ", &store);
    if (!next)
        return false; /* nothing to display */

    lines[0] = next;
    while (true)
    {
        screen->getstringsize(next, &w, NULL);
        if (lastbreak)
        {
            int next_w = (next - lastbreak) * space_w;

            if (x + next_w + w > vp->width - RECT_SPACING*2)
            {   /* too wide, wrap */
                if (x > maxw)
                    maxw = x;
                if ((y + h > vp->height) || (line >= (MAXLINES-1)))
                    break;  /* screen full or out of lines */
                x = 0;
                y += h;
                lines[++line] = next;
            }
            else
            {
                /*  restore & calculate spacing */
                *lastbreak = ' ';
                x += next_w;
            }
        }
        x += w;
        lastbreak = next + strlen(next);
        next = strtok_r(NULL, " ", &store);
        if (!next)
        {   /* no more words */
            if (x > maxw)
                maxw = x;
            break;
        }
    }

    /* prepare viewport
     * First boundaries, then the grey filling, then the black border and finally
     * the text*/

    screen->scroll_stop();

    width = maxw + 2*RECT_SPACING;
    height = y + 2*RECT_SPACING;

    if (width > vp->width)
        width = vp->width;
    if (height > vp->height)
        height = vp->height;

    vp->x += (vp->width - width) / 2;
    vp->y += (vp->height - height) / 2;
    vp->width = width;
    vp->height = height;

    vp->flags |=  VP_FLAG_ALIGN_CENTER;
#if LCD_DEPTH > 1
    if (screen->depth > 1)
    {
        vp->drawmode = DRMODE_FG;
        /* can't do vp->fg_pattern here, since set_foreground does a bit more on
         * greyscale */
        screen->set_foreground(SCREEN_COLOR_TO_NATIVE(screen, LCD_LIGHTGRAY));
    }
    else
#endif
        vp->drawmode = (DRMODE_SOLID|DRMODE_INVERSEVID);

    screen->fill_viewport();

#if LCD_DEPTH > 1
    if (screen->depth > 1)
        /* can't do vp->fg_pattern here, since set_foreground does a bit more on
         * greyscale */
        screen->set_foreground(SCREEN_COLOR_TO_NATIVE(screen, LCD_BLACK));
    else
#endif
        vp->drawmode = DRMODE_SOLID;

    screen->draw_border_viewport();

    /* prepare putting the text */
    y = RECT_SPACING;

    /* print the message to screen */
    for (i = 0; i <= line; i++, y+=h)
    {
        screen->putsxy(0, y, lines[i]);
    }
    return true; /* needs update */
}

void splashf(int ticks, const char *fmt, ...)
{
    va_list ap;

    /* If fmt is a lang ID then get the corresponding string (which
       still might contain % place holders). */
    fmt = P2STR((unsigned char *)fmt);
    FOR_NB_SCREENS(i)
    {
        struct screen * screen = &(screens[i]);
        struct viewport vp;
        viewport_set_defaults(&vp, screen->screen_type);
        struct viewport *last_vp = screen->set_viewport(&vp);

        va_start(ap, fmt);
        if (splash_internal(screen, fmt, ap, &vp, 0))
            screen->update_viewport();
        va_end(ap);

        screen->set_viewport(last_vp);
    }
    if (ticks)
        sleep(ticks);
}

void splash(int ticks, const char *str)
{
#if !defined(SIMULATOR)
    long id;
    /* fmt may be a so called virtual pointer. See settings.h. */
    if((id = P2ID((const unsigned char*)str)) >= 0)
        /* If fmt specifies a voicefont ID, and voice menus are
           enabled, then speak it. */
        cond_talk_ids_fq(id);
#endif
    splashf(ticks, "%s", P2STR((const unsigned char*)str));
}

/* splash a progress meter */
void splash_progress(int current, int total, const char *fmt, ...)
{
    va_list ap;
    int vp_flag = VP_FLAG_VP_DIRTY;
    /* progress update tick */
    static long next_tick = 0;
    long now = current_tick;

    if (current < total)
    {
        if(TIME_BEFORE(now, next_tick))
            return;
        /* limit to 20fps */
        next_tick = now + HZ/20;
        vp_flag = 0; /* don't mark vp dirty to prevent flashing */
    }

    /* If fmt is a lang ID then get the corresponding string (which
       still might contain % place holders). */
    fmt = P2STR((unsigned char *)fmt);
    FOR_NB_SCREENS(i)
    {
        struct screen * screen = &(screens[i]);
        struct viewport vp;
        viewport_set_defaults(&vp, screen->screen_type);
        struct viewport *last_vp = screen->set_viewport_ex(&vp, vp_flag);

        va_start(ap, fmt);
        if (splash_internal(screen, fmt, ap, &vp, 1))
        {
            int size = screen->getcharheight();
            int x = RECT_SPACING;
            int y = vp.height - size - RECT_SPACING;
            int w = vp.width - RECT_SPACING * 2;
            int h = size;
#ifdef HAVE_LCD_COLOR
            const int sb_flags = HORIZONTAL | FOREGROUND;
#else
            const int sb_flags = HORIZONTAL;
#endif
            gui_scrollbar_draw(screen, x, y, w, h, total, 0, current, sb_flags);

            screen->update_viewport();
        }
        va_end(ap);

        screen->set_viewport(last_vp);
    }
}
