/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * !!! DON'T MESS WITH THIS CODE UNLESS YOU'RE ABSOLUTELY SURE WHAT YOU DO !!!
 * 
 * Copyright (C) 2006 by Miika Pekkarinen
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "plugin.h"

/* All CFI flash routines are copied and ported from firmware_flash.c */

#ifndef SIMULATOR /* only for target */

unsigned char *audiobuf;
int audiobuf_size;

#if defined(IRIVER_H120)
#define PLATFORM_ID ID_IRIVER_H100
#else
#undef PLATFORM_ID /* this platform is not (yet) flashable */
#endif

#ifdef PLATFORM_ID

PLUGIN_HEADER

#if CONFIG_KEYPAD == IRIVER_H100_PAD
#define KEY1 BUTTON_OFF
#define KEY2 BUTTON_ON
#define KEY3 BUTTON_SELECT
#define KEYNAME1 "[Stop]"
#define KEYNAME2 "[On]"
#define KEYNAME3 "[Select]"
#endif

struct flash_info
{
    uint8_t manufacturer;
    uint8_t id;
    int size;
    char name[32];
};

static struct plugin_api* rb; /* here is a global api struct pointer */

#ifdef IRIVER_H100_SERIES
#define SEC_SIZE 4096
#define BOOTLOADER_ERASEGUARD  (BOOTLOADER_ENTRYPOINT / SEC_SIZE - 1)

static volatile uint16_t* FB = (uint16_t*)0x00000000; /* Flash base address */
#endif

/* read the manufacturer and device ID */
bool cfi_read_id(volatile uint16_t* pBase, uint8_t* pManufacturerID, uint8_t* pDeviceID)
{
    uint8_t not_manu, not_id; /* read values before switching to ID mode */
    uint8_t manu, id; /* read values when in ID mode */
    
    pBase = (uint16_t*)((uint32_t)pBase & 0xFFF80000); /* down to 512k align */
    
    /* read the normal content */
    not_manu = pBase[0]; /* should be 'A' (0x41) and 'R' (0x52) */
    not_id   = pBase[1]; /*  from the "ARCH" marker */
    
    pBase[0x5555] = 0xAA; /* enter command mode */
    pBase[0x2AAA] = 0x55;
    pBase[0x5555] = 0x90; /* ID command */
    rb->sleep(HZ/50); /* Atmel wants 20ms pause here */
    
    manu = pBase[0];
    id   = pBase[1];
    
    pBase[0] = 0xF0; /* reset flash (back to normal read mode) */
    rb->sleep(HZ/50); /* Atmel wants 20ms pause here */
    
    /* I assume success if the obtained values are different from
    the normal flash content. This is not perfectly bulletproof, they 
    could theoretically be the same by chance, causing us to fail. */
    if (not_manu != manu || not_id != id) /* a value has changed */
    {
        *pManufacturerID = manu; /* return the results */
        *pDeviceID = id;
        return true; /* success */
    }
    return false; /* fail */
}


/* erase the sector which contains the given address */
bool cfi_erase_sector(volatile uint16_t* pAddr)
{
    unsigned timeout = 430000; /* the timeout loop should be no less than 25ms */
    
    FB[0x5555] = 0xAA; /* enter command mode */
    FB[0x2AAA] = 0x55;
    FB[0x5555] = 0x80; /* erase command */
    FB[0x5555] = 0xAA; /* enter command mode */
    FB[0x2AAA] = 0x55;
    *pAddr = 0x30; /* erase the sector */

    /* I counted 7 instructions for this loop -> min. 0.58 us per round */
    /* Plus memory waitstates it will be much more, gives margin */
    while (*pAddr != 0xFFFF && --timeout); /* poll for erased */

    return (timeout != 0);
}


/* address must be in an erased location */
inline bool cfi_program_word(volatile uint16_t* pAddr, uint16_t data)
{
    unsigned timeout = 85; /* the timeout loop should be no less than 20us */
    
    if (~*pAddr & data) /* just a safety feature, not really necessary */
        return false; /* can't set any bit from 0 to 1 */
    
    FB[0x5555] = 0xAA; /* enter command mode */
    FB[0x2AAA] = 0x55;
    FB[0x5555] = 0xA0; /* byte program command */
    
    *pAddr = data;
    
    /* I counted 7 instructions for this loop -> min. 0.58 us per round */
    /* Plus memory waitstates it will be much more, gives margin */
    while (*pAddr != data && --timeout); /* poll for programmed */
    
    return (timeout != 0);
}


/* this returns true if supported and fills the info struct */
bool cfi_get_flash_info(struct flash_info* pInfo)
{
    rb->memset(pInfo, 0, sizeof(struct flash_info));
    
    if (!cfi_read_id(FB, &pInfo->manufacturer, &pInfo->id))
        return false;
    
    if (pInfo->manufacturer == 0xBF) /* SST */
    {
        if (pInfo->id == 0xD6)
        {
            pInfo->size = 256* 1024; /* 256k */
            rb->strcpy(pInfo->name, "SST39VF020");
            return true;
        }
        else if (pInfo->id == 0xD7)
        {
            pInfo->size = 512* 1024; /* 512k */
            rb->strcpy(pInfo->name, "SST39VF040");
            return true;
        }
        else if (pInfo->id == 0x82)
        {
            pInfo->size = 2048* 1024; /* 2 MiB */
            rb->strcpy(pInfo->name, "SST39VF160");
            return true;
        }
        else
            return false;
    }
    return false;
}


/*********** Utility Functions ************/


/* Tool function to calculate a CRC32 across some buffer */
/* third argument is either 0xFFFFFFFF to start or value from last piece */
unsigned crc_32(unsigned char* buf, unsigned len, unsigned crc32)
{
    /* CCITT standard polynomial 0x04C11DB7 */
    static const unsigned crc32_lookup[16] = 
    {   /* lookup table for 4 bits at a time is affordable */
        0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 
        0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 
        0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 
        0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD
    };
    
    unsigned char byte;
    unsigned t;

    while (len--)
    {   
        byte = *buf++; /* get one byte of data */

        /* upper nibble of our data */
        t = crc32 >> 28; /* extract the 4 most significant bits */
        t ^= byte >> 4; /* XOR in 4 bits of data into the extracted bits */
        crc32 <<= 4; /* shift the CRC register left 4 bits */     
        crc32 ^= crc32_lookup[t]; /* do the table lookup and XOR the result */

        /* lower nibble of our data */
        t = crc32 >> 28; /* extract the 4 most significant bits */
        t ^= byte & 0x0F; /* XOR in 4 bits of data into the extracted bits */
        crc32 <<= 4; /* shift the CRC register left 4 bits */     
        crc32 ^= crc32_lookup[t]; /* do the table lookup and XOR the result */
    }
    
    return crc32;
}


/***************** User Interface Functions *****************/
int WaitForButton(void)
{
    int button;
    
    do
    {
        button = rb->button_get(true);
    } while (button & BUTTON_REL);
    
    return button;
}

/* helper for DoUserDialog() */
void ShowFlashInfo(struct flash_info* pInfo)
{
    char buf[32];
    
    if (!pInfo->manufacturer)
    {
        rb->lcd_puts(0, 0, "Flash: M=?? D=??");
        rb->lcd_puts(0, 1, "Impossible to program");
    }
    else
    {
        rb->snprintf(buf, sizeof(buf), "Flash: M=%02x D=%02x",
            pInfo->manufacturer, pInfo->id);
        rb->lcd_puts(0, 0, buf);
        
        
        if (pInfo->size)
        {
            rb->lcd_puts(0, 1, pInfo->name);
            rb->snprintf(buf, sizeof(buf), "Size: %d KB", pInfo->size / 1024);
            rb->lcd_puts(0, 2, buf);
        }
        else
        {
            rb->lcd_puts(0, 1, "Unsupported chip");
        }
        
    }
    
    rb->lcd_update();
}

int load_firmware_file(const char *filename, uint32_t *checksum)
{
    int fd;
    int len, rc;
    int i;
    uint32_t sum;
    
    fd = rb->open(filename,  O_RDONLY);
    if (fd < 0)
        return -1;
    
    len = rb->filesize(fd);
    
    if (audiobuf_size < len)
    {
        rb->splash(HZ*3, true, "Out of memory!");
        rb->close(fd);
        return -2;
    }
    
    rb->read(fd, checksum, 4);
    rb->lseek(fd, FIRMWARE_OFFSET_FILE_DATA, SEEK_SET);
    len -= FIRMWARE_OFFSET_FILE_DATA;
    
    rc = rb->read(fd, audiobuf, len);
    rb->close(fd);
    if (rc != len)
    {
        rb->splash(HZ*3, true, "Read failure");
        return -3;
    }
    
    /* Verify the checksum */
    sum = 0;
    for (i = 0; i < len; i++)
        sum += audiobuf[i];
    
    if (sum != *checksum)
    {
        rb->splash(HZ*3, true, "Checksums mismatch!");
        return -4;
    }
    
    return len;
}

int flash_rockbox(const char *filename)
{
    struct flash_header hdr;
    char buf[32];
    int pos, i, len, rc;
    unsigned long checksum, sum;
    unsigned char *p8;
    uint16_t *p16;
    
    len = load_firmware_file(filename, &checksum);
    if (len < 0)
        return len * 10;
    
    /* Erase the program flash. */
    for (i = 1; i < BOOTLOADER_ERASEGUARD && (i-1)*4096 < len + 32; i++)
    {
        rc = cfi_erase_sector(FB + (SEC_SIZE/2) * i);
        rb->snprintf(buf, sizeof(buf), "Erase: 0x%03x  (%d)", i, rc);
        rb->lcd_puts(0, 3, buf);
        rb->lcd_update();
    }
    
    /* Write the magic and size. */
    rb->memset(&hdr, 0, sizeof(struct flash_header));
    hdr.magic = FLASH_MAGIC;
    hdr.length = len;
    // rb->strncpy(hdr.version, APPSVERSION, sizeof(hdr.version)-1);
    p16 = (uint16_t *)&hdr;
    
    rb->snprintf(buf, sizeof(buf), "Programming");
    rb->lcd_puts(0, 4, buf);
    rb->lcd_update();
    
    pos = FLASH_ENTRYPOINT/2;
    for (i = 0; i < (long)sizeof(struct flash_header)/2; i++)
    {
        cfi_program_word(FB + pos, p16[i]);
        pos++;
    }
    
    p16 = (uint16_t *)audiobuf;
    for (i = 0; i < len/2 && pos < (BOOTLOADER_ENTRYPOINT/2); i++)
        cfi_program_word(FB + pos + i, p16[i]);
    
    /* Verify */
    p8 = (char *)FLASH_ENTRYPOINT;
    p8 += sizeof(struct flash_header);
    sum = 0;
    for (i = 0; i < len; i++)
        sum += p8[i];
    
    if (sum != checksum)
    {
        rb->splash(HZ*3, true, "Verify failed!");
        /* Erase the magic sector so bootloader does not try to load
         * rockbox from flash and crash. */
        cfi_erase_sector(FB + SEC_SIZE/2);
        return -5;
    }
    
    return 0;
}

void show_fatal_error(void)
{
    rb->splash(HZ*30, true, "Disable idle poweroff, connect AC power and DON'T TURN PLAYER OFF!!");
    rb->splash(HZ*30, true, "Contact Rockbox developers as soon as possible!");
    rb->splash(HZ*30, true, "Your device won't be bricked unless you turn off the power");
    rb->splash(HZ*30, true, "Don't use the device before further instructions from Rockbox developers");
}

int flash_bootloader(const char *filename)
{
    char buf[32];
    int pos, i, len, rc;
    unsigned long checksum, sum, crc32;
    unsigned char *p8;
    uint16_t *p16;
    
    (void)buf;
    len = load_firmware_file(filename, &checksum);
    if (len < 0)
        return len * 10;
    
    if (len > 0xFFFF)
    {
        rb->splash(HZ*3, true, "Too big bootloader");
        return -1;
    }
    
    /* Verify the crc32 checksum also. */
    crc32 = crc_32(audiobuf, len, 0xffffffff);
#if 0
    rb->snprintf(buf, sizeof buf, "crc32 = 0x%08x", crc32);
    rb->splash(HZ*10, true, buf);
#else
    if (crc32 != 0xa930906d)
    {
        rb->splash(HZ*3, true, "Untested bootloader");
        return -2;
    }
#endif
    rb->lcd_puts(0, 3, "Processing critical sections...");
    rb->lcd_update();

    /* Erase the boot sector and write a proper reset vector. */
    cfi_erase_sector(FB);
    p16 = (uint16_t *)audiobuf;
    for (i = 0; i < 4; i++)
        cfi_program_word(FB + i, p16[i]);
    
    /* Erase the bootloader flash section. */
    for (i = BOOTLOADER_ENTRYPOINT/SEC_SIZE; i < 0x200; i++)
        rc = cfi_erase_sector(FB + (SEC_SIZE/2) * i);
    
    pos = BOOTLOADER_ENTRYPOINT/2;
    p16 = (uint16_t *)audiobuf;
    for (i = 0; i < len/2; i++)
        cfi_program_word(FB + pos + i, p16[i]);
    
    /* Verify */
    p8 = (char *)BOOTLOADER_ENTRYPOINT;
    sum = 0;
    for (i = 0; i < len; i++)
        sum += p8[i];
    
    if (sum != checksum)
    {
        rb->splash(HZ*3, true, "Verify failed!");
        show_fatal_error();
        return -5;
    }
    
    p8 = (char *)FB;
    for (i = 0; i < 8; i++)
    {
        if (p8[i] != audiobuf[i])
        {
            rb->splash(HZ*3, true, "Bootvector corrupt!");
            show_fatal_error();
            break;
        }
    }
    
    return 0;
}

/* Kind of our main function, defines the application flow. */
void DoUserDialog(char* filename)
{
    struct flash_info fi;
    int rc; /* generic return code */

    /* this can only work if Rockbox runs in DRAM, not flash ROM */
    if ((uint16_t*)rb >= FB && (uint16_t*)rb < FB + 4096*1024) /* 4 MB max */
    {   /* we're running from flash */
        rb->splash(HZ*3, true, "Not from ROM");
        return; /* exit */
    }

    /* refuse to work if the power may fail meanwhile */
    if (!rb->battery_level_safe())
    {
        rb->splash(HZ*3, true, "Battery too low!");
        return; /* exit */
    }
    
    rb->lcd_setfont(FONT_SYSFIXED);

    rc = cfi_get_flash_info(&fi);
    ShowFlashInfo(&fi);
    if (fi.size == 0) /* no valid chip */
    {
        rb->splash(HZ*3, true, "Sorry!");
        return; /* exit */
    }

    /* Debug? */
#if 0
    rb->memcpy(&hdr, (uint8_t *)(FLASH_ENTRYPOINT), sizeof(struct flash_header));
    rb->snprintf(buf, sizeof(buf), "Magic: 0x%03x", hdr.magic);
    rb->lcd_puts(0, 3, buf);
    rb->snprintf(buf, sizeof(buf), "Size: 0x%03x", hdr.length);
    rb->lcd_puts(0, 4, buf);
    rb->lcd_update();
    rb->sleep(HZ*10);
    
    rb->memcpy(&hdr, (uint8_t *)(FLASH_ENTRYPOINT/2), sizeof(struct flash_header));
    rb->snprintf(buf, sizeof(buf), "Magic: 0x%03x", hdr.magic);
    rb->lcd_puts(0, 3, buf);
    rb->snprintf(buf, sizeof(buf), "Size: 0x%03x", hdr.length);
    rb->lcd_puts(0, 4, buf);
    rb->lcd_update();
    rb->sleep(HZ*10);
#endif
    
    /* Restore? */
#if 0
    fd = rb->open("/internal_rom_000000-1FFFFF.bin",  O_RDONLY);
    if (fd < 0)
        return ;
    len = rb->filesize(fd);
    
    /* Erase the program flash. */
    for (i = 1; i < 0x1EF; i++)
    {
        rc = cfi_erase_sector(FB + (SEC_SIZE/2) * i);
        rb->snprintf(buf, sizeof(buf), "Erase: 0x%03x  (%d)", i, rc);
        rb->lcd_puts(0, 3, buf);
        rb->lcd_update();
    }
    
    i = FLASH_ENTRYPOINT/2;
    rb->lseek(fd, i*2, SEEK_SET);
    len -= i*2 - 0xffff;
    for (; len > 0 && i < (0x1F0000/2); i++)
    {
        rb->read(fd, bytes, 2);
        cfi_program_word(FB + i, (bytes[0] << 8) | bytes[1]);
        len -= 2;
    }
    
    rb->close(fd);
    return ;
#endif
    
    if (filename == NULL)
    {
        rb->splash(HZ*3, true, "Please use this plugin with \"Open with...\"");
        return ;
    }
    
    audiobuf = rb->plugin_get_audio_buffer(&audiobuf_size);
    
    if (rb->strcasestr(filename, "/rockbox.iriver"))
        flash_rockbox(filename);
    else if (rb->strcasestr(filename, "/bootloader.iriver"))
        flash_bootloader(filename);
    else
        rb->splash(HZ*3, true, "Unknown file type");
}


/***************** Plugin Entry Point *****************/

enum plugin_status plugin_start(struct plugin_api* api, void* parameter)
{
    int oldmode;

    rb = api; /* copy to global api pointer */
    
    /* now go ahead and have fun! */
    oldmode = rb->system_memory_guard(MEMGUARD_NONE); /*disable memory guard */
    DoUserDialog((char*) parameter);
    rb->system_memory_guard(oldmode);              /* re-enable memory guard */

    return PLUGIN_OK;
}

#endif /* ifdef PLATFORM_ID */
#endif /* #ifndef SIMULATOR */
