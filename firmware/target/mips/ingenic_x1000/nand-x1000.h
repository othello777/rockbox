/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2021 Aidan MacDonald
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

#ifndef __NAND_X1000_H__
#define __NAND_X1000_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "kernel.h"

#define NAND_SUCCESS            0
#define NAND_ERR_UNKNOWN_CHIP   (-1)
#define NAND_ERR_PROGRAM_FAIL   (-2)
#define NAND_ERR_ERASE_FAIL     (-3)
#define NAND_ERR_UNALIGNED      (-4)
#define NAND_ERR_ECC_FAIL       (-5)

/* keep max page size in sync with the NAND chip table in the .c file */
#define NAND_DRV_SCRATCHSIZE 32
#define NAND_DRV_MAXPAGESIZE 2112

/* Quad I/O support bit */
#define NAND_CHIPFLAG_QUAD          0x0001
/* Chip requires QE bit set to enable quad I/O mode */
#define NAND_CHIPFLAG_HAS_QE_BIT    0x0002
/* True if the chip has on-die ECC */
#define NAND_CHIPFLAG_ON_DIE_ECC    0x0004

/*                                          cmd   mode             a  d  phase format         has data */
#define NANDCMD_RESET               SFC_CMD(0xff, SFC_TMODE_1_1_1, 0, 0, SFC_PFMT_ADDR_FIRST, 0)
#define NANDCMD_READID_OPCODE       SFC_CMD(0x9f, SFC_TMODE_1_1_1, 0, 0, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_READID_ADDR         SFC_CMD(0x9f, SFC_TMODE_1_1_1, 1, 0, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_READID_DUMMY        SFC_CMD(0x9f, SFC_TMODE_1_1_1, 0, 8, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_WR_EN               SFC_CMD(0x06, SFC_TMODE_1_1_1, 0, 0, SFC_PFMT_ADDR_FIRST, 0)
#define NANDCMD_GET_FEATURE         SFC_CMD(0x0f, SFC_TMODE_1_1_1, 1, 0, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_SET_FEATURE         SFC_CMD(0x1f, SFC_TMODE_1_1_1, 1, 0, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_PAGE_READ           SFC_CMD(0x13, SFC_TMODE_1_1_1, 3, 0, SFC_PFMT_ADDR_FIRST, 0)
#define NANDCMD_READ_CACHE_SLOW     SFC_CMD(0x03, SFC_TMODE_1_1_1, 2, 8, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_READ_CACHE          SFC_CMD(0x0b, SFC_TMODE_1_1_1, 2, 8, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_READ_CACHE_x4       SFC_CMD(0x6b, SFC_TMODE_1_1_4, 2, 8, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_PROGRAM_EXECUTE     SFC_CMD(0x10, SFC_TMODE_1_1_1, 3, 0, SFC_PFMT_ADDR_FIRST, 0)
#define NANDCMD_PROGRAM_LOAD        SFC_CMD(0x02, SFC_TMODE_1_1_1, 2, 0, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_PROGRAM_LOAD_x4     SFC_CMD(0x32, SFC_TMODE_1_1_4, 2, 0, SFC_PFMT_ADDR_FIRST, 1)
#define NANDCMD_BLOCK_ERASE         SFC_CMD(0xd8, SFC_TMODE_1_1_1, 3, 0, SFC_PFMT_ADDR_FIRST, 0)

/* Feature registers are found in linux/mtd/spinand.h,
 * apparently these are pretty standardized */
#define FREG_PROT        0xa0
#define FREG_PROT_UNLOCK 0x00

#define FREG_CFG             0xb0
#define FREG_CFG_OTP_ENABLE  (1 << 6)
#define FREG_CFG_ECC_ENABLE  (1 << 4)
#define FREG_CFG_QUAD_ENABLE (1 << 0)

/* Winbond-specific bit used on the W25N01GVxx */
#define FREG_CFG_WINBOND_BUF (1 << 3)

#define FREG_STATUS               0xc0
#define FREG_STATUS_BUSY          (1 << 0)
#define FREG_STATUS_EFAIL         (1 << 2)
#define FREG_STATUS_PFAIL         (1 << 3)
#define FREG_STATUS_ECC_MASK      (3 << 4)
#define FREG_STATUS_ECC_NO_FLIPS  (0 << 4)
#define FREG_STATUS_ECC_HAS_FLIPS (1 << 4)
#define FREG_STATUS_ECC_UNCOR_ERR (2 << 4)

/* Types to distinguish between block & page addresses in the API.
 *
 *                BIT 31                            log2_ppb bits
 *                +-------------------------------+---------------+
 *  nand_page_t = | block nr                      | page nr       |
 *                +-------------------------------+---------------+
 *                                                            BIT 0
 *
 * The page address is split into block and page numbers. Page numbers occupy
 * the lower log2_ppb bits, and the block number occupies the upper bits.
 *
 * Block addresses are structured the same as page addresses, but with a page
 * number of 0. So block number N has address N << log2_ppb.
 */
typedef uint32_t nand_block_t;
typedef uint32_t nand_page_t;

struct nand_drv;

struct nand_chip {
    /* Base2 logarithm of the number of pages per block */
    unsigned log2_ppb;

    /* Size of a page's main / oob areas, in bytes. */
    unsigned page_size;
    unsigned oob_size;

    /* Total number of blocks in the chip */
    unsigned nr_blocks;

    /* Bad block marker offset within the 1st page of a bad block */
    unsigned bbm_pos;

    /* Clock frequency to use */
    uint32_t clock_freq;

    /* Value of sfc_dev_conf */
    uint32_t dev_conf;

    /* Chip specific flags */
    uint32_t flags;

    /* SFC commands for issuing I/O ops */
    uint32_t cmd_page_read;
    uint32_t cmd_program_execute;
    uint32_t cmd_block_erase;
    uint32_t cmd_read_cache;
    uint32_t cmd_program_load;

    /* Chip-specific setup routine */
    void(*setup_chip)(struct nand_drv* drv);
};

enum nand_readid_method {
    NAND_READID_OPCODE,
    NAND_READID_ADDR,
    NAND_READID_DUMMY,
};

struct nand_chip_id {
    uint8_t method;
    uint8_t num_id_bytes;
    uint8_t id_bytes[4];
    const struct nand_chip* chip;
};

#define NAND_CHIP_ID(_chip, _method, ...) \
    { .method = _method, \
      .num_id_bytes = ARRAYLEN(((uint8_t[]){__VA_ARGS__})),   \
      .id_bytes = {__VA_ARGS__}, \
      .chip = _chip }

struct nand_drv {
    /* NAND access lock. Needs to be held during any operations. */
    struct mutex mutex;

    /* Reference count for open/close operations */
    unsigned refcount;

    /* Scratch and page buffers. Both need to be cacheline-aligned and are
     * provided externally by the caller prior to nand_open().
     *
     * - The scratch buffer is NAND_DRV_SCRATCHSIZE bytes long and is used
     *   for small data transfers associated with commands. It must not be
     *   disturbed while any NAND operation is in progress.
     *
     * - The page buffer is used by certain functions like nand_read_bytes(),
     *   but it's main purpose is to provide a common temporary buffer for
     *   driver users to perform I/O with. Must be fpage_size bytes long.
     */
    uint8_t* scratch_buf;
    uint8_t* page_buf;

    /* Pointer to the chip data. */
    const struct nand_chip* chip;

    /* Pages per block = 1 << chip->log2_ppb */
    unsigned ppb;

    /* Full page size = chip->page_size + chip->oob_size */
    unsigned fpage_size;
};

extern const struct nand_chip_id supported_nand_chips[];
extern const size_t nr_supported_nand_chips;

/* Return the static NAND driver instance.
 *
 * ALL normal Rockbox code should use this instance. The SPL does not
 * use it, because it needs to manually place buffers in external RAM.
 */
extern struct nand_drv* nand_init(void);

static inline void nand_lock(struct nand_drv* drv)
{
    mutex_lock(&drv->mutex);
}

static inline void nand_unlock(struct nand_drv* drv)
{
    mutex_unlock(&drv->mutex);
}

/* Open or close the NAND driver
 *
 * The NAND driver is reference counted, and opening / closing it will
 * increment and decrement the reference count. The hardware is only
 * controlled when the reference count rises above or falls to 0, else
 * these functions are no-ops which always succeed.
 *
 * These functions require the lock to be held.
 */
extern int nand_open(struct nand_drv* drv);
extern void nand_close(struct nand_drv* drv);

/* Enable/disable OTP access. OTP data pages are usually vendor-specific. */
void nand_enable_otp(struct nand_drv* drv, bool enable);

/* Read / program / erase operations. Buffer needs to be cache-aligned for DMA.
 * Read and program operate on full page data, ie. including OOB data areas.
 *
 * NOTE: ECC is not implemented. If it ever needs to be, these functions will
 * probably use ECC transparently. All code should be written to expect this.
 */
extern int nand_block_erase(struct nand_drv* drv, nand_block_t block);
extern int nand_page_program(struct nand_drv* drv, nand_page_t page, const void* buffer);
extern int nand_page_read(struct nand_drv* drv, nand_page_t page, void* buffer);

/* Wrappers to read/write bytes. For simple access to the main data area only.
 * The write address / length must align to a block boundary. Reads do not have
 * any alignment requirement. OOB data is never read, and is written as 0xff.
 */
extern int nand_read_bytes(struct nand_drv* drv, uint32_t byte_addr, uint32_t byte_len, void* buffer);
extern int nand_write_bytes(struct nand_drv* drv, uint32_t byte_addr, uint32_t byte_len, const void* buffer);

#endif /* __NAND_X1000_H__ */
