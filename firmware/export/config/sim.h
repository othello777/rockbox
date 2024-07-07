/* This config file is for all simulators, and is used in addition to the
   target specific configs */

#undef CONFIG_CPU

#undef HAVE_FMADC

#undef NEED_ATA_POWER_BATT_MEASURE

#undef CONFIG_I2C

#undef HAVE_PCM_DMA_ADDRESS
#undef HAVE_PCM_REC_DMA_ADDRESS

#undef FLASH_SIZE

#undef HAVE_ATA_POWER_OFF

#undef CONFIG_LCD
#undef LCD_DPI /* likely to be too different on a PC */

#undef CONFIG_LED

#undef ROM_START

#undef AMS_OF_SIZE

/* Simulator core requires hotswap */
#ifndef HAVE_HOTSWAP
#define HAVE_HOTSWAP
#endif

#undef HAVE_HOTSWAP_STORAGE_AS_MAIN
#undef HAVE_STORAGE_FLUSH

#undef CONFIG_NAND

#if defined(CONFIG_CHARGING) && CONFIG_CHARGING > CHARGING_MONITOR
#undef CONFIG_CHARGING
#define CONFIG_CHARGING CHARGING_MONITOR
#endif

/*
 * Effectively disable battery smoothing, our simulated charge/ discharge
 * cycle is way too fast for it otherwise
 */
#undef BATT_AVE_SAMPLES
#define BATT_AVE_SAMPLES 1

#undef CONFIG_USBOTG

#undef USB_HANDLED_BY_OF

#undef HAVE_USBSTACK
#undef USB_VENDOR_ID
#undef USB_PRODUCT_ID
#undef USB_NUM_ENDPOINTS
#undef HAVE_USB_HID_MOUSE

#undef HAVE_ADJUSTABLE_CPU_FREQ

#undef MI4_FORMAT
#undef BOOTFILE_EXT
#undef BOOTFILE
#undef BOOTDIR

#undef BOOTLOADER_ENTRYPOINT

#undef FLASH_ENTRYPOINT

#undef FLASH_MAGIC

#undef HAVE_EEPROM
#undef HAVE_EEPROM_SETTINGS

#undef HAVE_HARDWARE_BEEP

#undef HAVE_POWEROFF_WHILE_CHARGING

#undef INCLUDE_TIMEOUT_API

#undef HAVE_FLASHED_ROCKBOX

#undef IPOD_ACCESSORY_PROTOCOL

#undef HAVE_WHEEL_POSITION

#undef HAVE_LCD_MODES

#undef HAVE_SPEAKER

#undef BUTTON_DRIVER_CLOSE

#if     CONFIG_BACKLIGHT_FADING == BACKLIGHT_FADING_SW_HW_REG
#undef  CONFIG_BACKLIGHT_FADING
        /* simulate SW_SETTING, as we handle sdl very similary */
#define CONFIG_BACKLIGHT_FADING BACKLIGHT_FADING_SW_SETTING

#elif  (CONFIG_BACKLIGHT_FADING == BACKLIGHT_FADING_PWM || \
        CONFIG_BACKLIGHT_FADING == BACKLIGHT_FADING_TARGET)
        /* we don't simulate that yet */
#undef  CONFIG_BACKLIGHT_FADING
#endif

#ifdef  HAVE_BACKLIGHT_BRIGHTNESS
#undef  DEFAULT_BRIGHTNESS_SETTING
/* default for 100% in the sim */
#define DEFAULT_BRIGHTNESS_SETTING MAX_BRIGHTNESS_SETTING
#endif

#define CONFIG_PLATFORM (PLATFORM_HOSTED|PLATFORM_SDL)
#define HAVE_SDL
#define HAVE_SDL_AUDIO

#define _ISOC99_SOURCE 1
