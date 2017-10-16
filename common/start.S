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
	l.jal	main
	l.nop
1:	l.j	1b
	l.nop
endfunc start
