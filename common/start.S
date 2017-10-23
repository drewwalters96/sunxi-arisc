/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <assembler.S>
#include <platform/memory.h>

func start
	/* Put the stack at the top of SRAM A2. */
	l.movhi	r1, hi(SRAM_A2_ARISC_BASE + SRAM_A2_SIZE - 4)
	l.ori	r1, r1, lo(SRAM_A2_ARISC_BASE + SRAM_A2_SIZE - 4)
	/* Clear BSS. */
	l.movhi	r3, hi(__bss_start)
	l.ori	r3, r3, lo(__bss_start)
	l.movhi	r4, hi(__bss_end)
	l.ori	r4, r4, lo(__bss_end)
1:	l.sfltu	r3, r4
	l.bnf	2f
	l.sw	0x0(r3), r0
	l.sw	0x4(r3), r0
	l.sw	0x8(r3), r0
	l.sw	0xc(r3), r0
	l.j	1b
	l.addi	r3, r3, 0x10
2:	l.jal	main
	l.nop
3:	l.j	3b
	l.nop
endfunc start
