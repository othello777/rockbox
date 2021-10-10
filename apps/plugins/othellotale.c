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
#include "plugin.h"
#include "pluginbitmaps/othellotale_human.h"
//#include "lib/display_text.h"
//#include "lib/mylcd.h"

#define SCR_BACK 		BUTTON_SCROLL_BACK
#define SCR_FWD 		BUTTON_SCROLL_FWD
#define RFK_SELECT      BUTTON_SELECT
#define RFK_UP          BUTTON_MENU
#define RFK_DOWN        BUTTON_PLAY
#define RFK_RIGHT       BUTTON_RIGHT
#define RFK_LEFT        BUTTON_LEFT

enum key {
    UP,
    DOWN,
    LEFT,
	RIGHT
};

extern const fb_data othellotale_human[];

static bool IsKeyDown(int key)
{
	int button = rb->button_status();
	switch (button)
	{
	case RFK_UP:
		if(key == 0)
			return true;
	  break;
	case RFK_DOWN:
		if(key == 1)
			return true;
	  break;
	case RFK_LEFT:
		if(key == 2)
			return true;
	  break;
	case RFK_RIGHT:
		if(key == 3)
			return true;
	  break;
	}
	return false;
}

enum plugin_status plugin_start(const void* parameter)
{
    /* if you don't use the parameter, you can do like
       this to avoid the compiler warning about it */
    (void)parameter;

    int facing = 0;
    int walkmod = 0;
    int locx = 0;
    int locy = 0;
    int speed = 2;
    int walktimer = 0;
    int interval = 6;
    int sprwidth = 19;
    int sprheight = 29;
    bool notleave = true;
    while(notleave)
    {
    	if(IsKeyDown(UP))
    	{
    		locy -= speed;
    		facing = UP;
    		walktimer++;
    	}
    	else if(IsKeyDown(DOWN))
		{
			locy += speed;
			facing = DOWN;
			walktimer++;
		}
    	else if(IsKeyDown(LEFT))
		{
			locx -= speed;
			facing = LEFT;
			walktimer++;
		}
    	else if(IsKeyDown(RIGHT))
		{
			locx += speed;
			facing = RIGHT;
			walktimer++;
		}
    	else
    	{
    		walktimer = 0;
    		walkmod = 0;
    	}

    	if(walktimer >= interval)
    	{
    		if(walkmod < 1)
    			walkmod = 1;
    		else if(walkmod > 0)
    			walkmod = 0;
    		walktimer = 1;
    	}


    	rb->lcd_clear_display();
    	if(facing == UP)
    	{
    		int offset = 0;
    		if(walktimer > 0)
    			offset = 1;
    		rb->lcd_bitmap_part(othellotale_human, sprwidth * (7 + walkmod + offset), 0,
    				STRIDE(SCREEN_MAIN, BMPWIDTH_othellotale_human, BMPHEIGHT_othellotale_human), locx, locy, sprwidth, sprheight);
    	}
    	if(facing == DOWN)
		{
			int offset = 0;
			if(walktimer > 0)
				offset = 1;
			rb->lcd_bitmap_part(othellotale_human, sprwidth * (walkmod + offset), 0,
					STRIDE(SCREEN_MAIN, BMPWIDTH_othellotale_human, BMPHEIGHT_othellotale_human), locx, locy, sprwidth, sprheight);
		}
    	if(facing == LEFT)
		{
			rb->lcd_bitmap_part(othellotale_human, sprwidth * (3 + walkmod), 0,
					STRIDE(SCREEN_MAIN, BMPWIDTH_othellotale_human, BMPHEIGHT_othellotale_human), locx, locy, sprwidth, sprheight);
		}
    	if(facing == RIGHT)
		{
			rb->lcd_bitmap_part(othellotale_human, sprwidth * (5 + walkmod), 0,
					STRIDE(SCREEN_MAIN, BMPWIDTH_othellotale_human, BMPHEIGHT_othellotale_human), locx, locy, sprwidth, sprheight);
		}

		rb->lcd_update();
		rb->sleep(HZ/40);
		if(rb->button_status() == RFK_SELECT)
			notleave = false;
    }
    //rb->splash(HZ*2, "howdy!");

    return PLUGIN_OK;
}
