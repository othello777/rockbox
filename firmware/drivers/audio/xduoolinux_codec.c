/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 *
 * Copyright (c) 2018 Marcin Bukat
 * Copyright (c) 2018 Roman Stolyarov
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

//#define LOGF_ENABLE

#include "config.h"
#include "audio.h"
#include "audiohw.h"
#include "button.h"
#include "system.h"
#include "kernel.h"
#include "panic.h"
#include "sysfs.h"
#include "alsa-controls.h"
#include "pcm-alsa.h"

#include "logf.h"

static int fd_hw;
static int inited = 0;

static long int vol_l_hw = 255;
static long int vol_r_hw = 255;
static long int last_ps = -1;

static void hw_open(void)
{
    fd_hw = open("/dev/snd/controlC0", O_RDWR);
    if(fd_hw < 0)
        panicf("Cannot open '/dev/snd/controlC0'");
}

static void hw_close(void)
{
    close(fd_hw);
}

static int muted = -1;

void audiohw_mute(int mute)
{
    logf("mute %d", mute);

    if (muted == mute)
       return;

    muted = mute;

    if(mute)
    {
        long int ps0 = 0;
        alsa_controls_set_ints("Output Port Switch", 1, &ps0);
    }
    else
    {
        xduoo_get_outputs();
    }
}

int xduoo_get_outputs(void){
    long int ps = 0; // Muted, if nothing is plugged in!

    int status = 0;

    if (!inited) return ps;

    const char * const sysfs_lo_switch = "/sys/class/switch/lineout/state";
    const char * const sysfs_hs_switch = "/sys/class/switch/headset/state";
#if defined(XDUOO_X20)
    const char * const sysfs_bal_switch = "/sys/class/switch/balance/state";
#endif

    sysfs_get_int(sysfs_lo_switch, &status);
    if (status) ps = 1; // lineout

    sysfs_get_int(sysfs_hs_switch, &status);
    if (status) ps = 2; // headset

#if defined(XDUOO_X20)
    sysfs_get_int(sysfs_bal_switch, &status);
    if (status) ps = 3; // balance
#endif

    xduoo_set_output(ps);

    return ps;
}

void xduoo_set_output(int ps)
{
    if (!inited || muted) return;

    if (last_ps != ps)
    {
        logf("set out %d/%d", ps, last_ps);
        /* Output port switch */
        last_ps = ps;
        alsa_controls_set_ints("Output Port Switch", 1, &last_ps);
	audiohw_set_volume(vol_l_hw, vol_r_hw);
    }
}

void audiohw_preinit(void)
{
    logf("hw preinit");
    alsa_controls_init();
    hw_open();
    audiohw_mute(true);  /* Start muted to avoid the POP */
    inited = 1;
//    const char * const codec_pmdown = "/sys/devices/platform/ingenic-x3ii.0/x3ii-ak4490-i2s/pmdown_time";  // in ms, defaults 5000
}

void audiohw_postinit(void)
{
    logf("hw postinit");
}

void audiohw_close(void)
{
    logf("hw close");
    inited = 0;
    hw_close();
    alsa_controls_close();
}

void audiohw_set_frequency(int fsel)
{
    (void)fsel;
}

void audiohw_set_volume(int vol_l, int vol_r)
{
    logf("hw vol %d %d", vol_l, vol_r);

    long l,r;

    vol_l_hw = vol_l;
    vol_r_hw = vol_r;

    if (lineout_inserted()) {
        l = 0;
        r = 0;
    } else {
        l = -vol_l/5;
        r = -vol_r/5;
    }

    alsa_controls_set_ints("Left Playback Volume", 1, &l);
    alsa_controls_set_ints("Right Playback Volume", 1, &r);
}

void audiohw_set_lineout_volume(int vol_l, int vol_r)
{
    long l,r;

    logf("lo vol %d %d", vol_l, vol_r);

    (void)vol_l;
    (void)vol_r;

    if (lineout_inserted()) {
        l = 0;
        r = 0;
    } else {
        l = -vol_l_hw/5;
        r = -vol_r_hw/5;
    }

    alsa_controls_set_ints("Left Playback Volume", 1, &l);
    alsa_controls_set_ints("Right Playback Volume", 1, &r);
}

void audiohw_set_filter_roll_off(int value)
{
    logf("rolloff %d", value);
    /* 0 = Sharp;
       1 = Slow;
       2 = Short Sharp
       3 = Short Slow */
#if defined(XDUOO_X3II)
    long int value_hw = value;
    alsa_controls_set_ints("AK4490 Digital Filter", 1, &value_hw);
#elif defined(XDUOO_X20)
    long int value_hw = value;
    alsa_controls_set_ints("ES9018_K2M Digital Filter", 1, &value_hw);
#else
    (void)value;
#endif
}
