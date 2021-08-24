/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Björn Stenberg
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

/* welcome to the example rockbox plugin */

/* mandatory include for all plugins */
#include "plugin.h"
#define AMP 32000

static void printlines(char *input)
{
	char str[rb->strlen(input)];
	rb->strcpy(str, input);
	char *save;
	char delim[] = "\n";

	char *ptr = rb->strtok_r(str, delim, &save);

	int line = 0;
	rb->lcd_clear_display();
	while(ptr != NULL)
	{
		rb->lcd_puts(0, line, ptr);
		ptr = rb->strtok_r(NULL, delim, &save);
		line = line + 1;
	}
	rb->lcd_update();
}

static void bsleep(int time)
{
	printlines("sleep\nsleep\nsleep");
	rb->sleep(HZ * time / 1000);
}

static void beep(int freq, int time)
{
	printlines("\n\nbeep\nbeep");

	rb->beep_play(freq, (HZ/100)*time, AMP);
	rb->sleep(HZ * time / 1000);
	//bsleep(time);
}

static void playtune(void)
{
	beep(196,200);
	beep(196,200);
	beep(233,200);
	beep(247,200);
	beep(262,200);
	beep(247,200);
	beep(233,200);
	beep(220,200);
	beep(196,200);
	beep(196,200);
	beep(233,200);
	beep(247,200);
	beep(262,200);
	beep(247,200);
	beep(233,200);
	beep(220,200);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	bsleep(200);
	beep(784,100);
	bsleep(300);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(123,100);
	beep(123,100);
	beep(123,200);
	beep(131,100);
	beep(131,100);
	beep(131,200);
	beep(139,100);
	beep(139,100);
	beep(139,200);
	beep(147,100);
	beep(147,100);
	beep(147,200);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	bsleep(600);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(466, 100);
	bsleep(100);
	beep(494, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(392, 100);
	bsleep(100);
	beep(440, 100);
	bsleep(100);
	beep(1245, 50);
	beep(831, 25);
	bsleep(75);
	beep(831, 50);
	beep(587, 25);
	bsleep(75);
	beep(659, 75);
	bsleep(50);
	beep(494, 25);
	beep(622, 125);
	bsleep(100);
	beep(262, 25);
	bsleep(75);
	beep(831, 50);
	beep(440, 25);
	bsleep(50);
	beep(131, 25);
	beep(1175, 25);
	bsleep(175);
	beep(1245, 25);
	beep(311, 100);
	bsleep(25);
	beep(659, 25);
	beep(277, 75);
	beep(220, 25);
	bsleep(100);
	beep(185, 25);
	bsleep(75);
	beep(220,100);
	bsleep(100);
	beep(311,25);
	bsleep(50);
	beep(277,25);
	bsleep(50);
	beep(247,25);
	bsleep(50);
	beep(220,25);
	bsleep(50);
	beep(233,25);
	bsleep(50);
	beep(196,25);
	bsleep(200);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	bsleep(600);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(220, 100);
	bsleep(100);
	beep(185, 100);
	bsleep(100);
	beep(220, 100);
	bsleep(100);
	beep(247, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(247, 100);
	bsleep(300);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(175, 400);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(185, 400);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(220, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(294, 100);
	bsleep(300);
	beep(262, 100);
	bsleep(300);
	beep(131, 200);
	bsleep(200);
	beep(165, 800);
	beep(147, 400);
	beep(196, 400);
	beep(131, 1200);
	bsleep(400);
	beep(220, 800);
	beep(196, 400);
	beep(262, 400);
	beep(175, 1200);
	bsleep(400);
	beep(165, 800);
	beep(147, 400);
	beep(196, 400);
	beep(131, 1200);
	bsleep(400);
	beep(220, 800);
	beep(196, 400);
	beep(247, 400);
	beep(262, 1200);
	bsleep(400);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(175, 400);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(185, 400);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(311, 100);
	bsleep(100);
	beep(330, 100);
	bsleep(100);
	beep(294, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(100);
	beep(220, 100);
	bsleep(100);
	beep(262, 100);
	bsleep(300);
	beep(294, 100);
	bsleep(300);
	beep(262, 100);
	bsleep(300);
	beep(131, 300);
	bsleep(100);
	beep(131, 200);
	beep(131, 200);
	beep(156, 200);
	beep(165, 200);
	beep(175, 200);
	beep(175, 200);
	beep(185, 200);
	beep(196, 200);
	beep(131, 200);
	beep(131, 200);
	beep(156, 200);
	beep(165, 200);
	beep(175, 200);
	beep(175, 200);
	beep(185, 200);
	beep(196, 200);
	beep(131, 200);
	beep(131, 200);
	beep(156, 200);
	beep(165, 200);
	beep(175, 200);
	beep(175, 200);
	beep(185, 200);
	beep(196, 200);
	beep(311, 200);
	beep(294, 200);
	beep(277, 200);
	beep(262, 200);
	beep(247, 200);
	beep(233, 200);
	beep(220, 200);
	beep(208, 200);
	beep(131, 200);
	beep(131, 200);
	beep(156, 200);
	beep(165, 200);
	beep(175, 200);
	beep(175, 200);
	beep(185, 200);
	beep(196, 200);
	beep(131, 200);
	beep(131, 200);
	beep(156, 200);
	beep(165, 200);
	beep(175, 200);
	beep(175, 200);
	beep(185, 200);
	beep(196, 200);
	beep(131, 200);
	beep(131, 200);
	beep(156, 200);
	beep(165, 200);
	beep(175, 200);
	beep(175, 200);
	beep(185, 200);
	beep(196, 200);
	beep(311, 200);
	beep(294, 200);
	beep(277, 200);
	beep(262, 200);
	beep(247, 200);
	beep(233, 200);
	beep(220, 200);
	beep(208, 200);
	beep(196, 200);
	beep(185, 200);
	bsleep(1200);
	beep(196, 200);
	beep(185, 200);
	bsleep(1200);
	beep(196, 200);
	beep(185, 200);
	bsleep(1200);
	beep(208, 200);
	bsleep(1400);
}

/* this is
 *the plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
    /* if you don't use the parameter, you can do like
       this to avoid the compiler warning about it */
    (void)parameter;
    //rb->keyclick_click();
    rb->splash(HZ, "Sing me a song");

    playtune();

    /* "rb->" marks a plugin api call. Rockbox offers many of its built-in
     * functions to plugins */
    /* now go ahead and have fun! */
    rb->splash(HZ*2, "Hello Rockbox~!");

    /* tell Rockbox that we have completed successfully */
    return PLUGIN_OK;
}
