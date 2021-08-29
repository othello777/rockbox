/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2021 othello7
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

// mandatory include for all plugins
#include "plugin.h"
#define AMP 32000

#define SCR_BACK 		BUTTON_SCROLL_BACK
#define SCR_FWD 		BUTTON_SCROLL_FWD
#define RFK_SELECT      BUTTON_SELECT
#define RFK_UP          BUTTON_MENU
#define RFK_DOWN        BUTTON_PLAY
#define RFK_RIGHT       BUTTON_RIGHT
#define RFK_LEFT        BUTTON_LEFT

static char slog[7][23];
static int initfill = 0;
static int vl = 5;
static bool q = false;
//static int textlimit = 40;

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

static void addlog(char *line)
{
	//puts("log1");
	if(initfill > vl - 1)
	{
		//puts("log2");
		int n = 1;
		while(n <= vl)
		{
			rb->strcpy(slog[n - 1], slog[n]);
			n = n + 1;
		}
		rb->strcpy(slog[vl], line);

		rb->strcpy(slog[initfill], line);
		char lined[127] = "";
		int i = 0;
		while(i <= vl)
		{
			rb->strcat(lined, "\n");
			rb->strcat(lined, slog[i]);
			i = i + 1;
		}
		printlines(lined);
	}
	else
	{
		//puts("log3");
		rb->strcpy(slog[initfill], line);
		char lined[40] = "";
		int i = 0;
		//puts("log4");
		while(i <= vl)
		{
			rb->strcat(lined, "\n");
			rb->strcat(lined, slog[i]);
			i = i + 1;
		}
		//puts("log5");
		printlines(lined);
		initfill = initfill + 1;
	}
}

static void quitcheck(void)
{
	if(rb->button_status() == RFK_UP || rb->button_status() == RFK_LEFT)
	{
		addlog("quitting");
		q = true;
	}
}

static void bsleep(int time)
{
	quitcheck();
	if(q)
			return;
	char prin[11] = "sleep";
	//int *ls = {(int)time};
	rb->snprintf(prin, 10, "sleep %d", time);
	addlog(prin);
	rb->sleep(HZ * time / 1000);
}

static void beep(int freq, int time)
{
	quitcheck();
	if(q)
		return;
	//char pre[20] = "";
	char prin[23] = "";
	rb->snprintf(prin, 22, "beep %d %d", time, freq);
	//char prin[20] = "";
	//rb->snprintf(prin, 19, "%d", freq);
	//rb->strcat(pre, prin);
	addlog(prin);

	rb->beep_play(freq, (HZ/100)*time, AMP);
	rb->sleep(HZ * time / 1000);
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

// this is the plugin entry point
enum plugin_status plugin_start(const void* parameter)
{
    /* if you don't use the parameter, you can do like
       this to avoid the compiler warning about it */
    (void)parameter;

    rb->splash(HZ/2, "Sing me a song");

    playtune();

    // "rb->" marks a plugin api call. Rockbox offers many of its built-in functions to plugins
    // now go ahead and have fun!
    rb->splash(HZ*2, "Hello Rockbox~!");

    // tell Rockbox that we have completed successfully
    return PLUGIN_OK;
}
