/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef PLATFORM_MEMORY_H
#define PLATFORM_MEMORY_H

#define BROM_ARISC_BASE			0x00040000
#define BROM_BASE			0x00000000
#define BROM_SIZE			0x00010000
#define SRAM_A1_ARISC_BASE		/* none */
#define SRAM_A1_BASE			0x00010000
#define SRAM_A1_SIZE			0x00008000
#define SRAM_A2_ARISC_BASE		0x00004000
#define SRAM_A2_BASE			0x00044000
#define SRAM_A2_SIZE			0x00010000
#define DRAM_BASE			0x40000000
#define DRAM_SIZE			0xc0000000

#endif /* PLATFORM_MEMORY_H */
