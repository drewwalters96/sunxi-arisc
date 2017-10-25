/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <assembler.S>
#include <util.h>
#include <platform/devices.h>

func early_console_init
	l.jr	r9
	l.nop
endfunc early_console_init

func early_console_putc
	l.movhi	r4, hi(DEV_UART0)
	l.j	early_uart_putc
	l.ori	r4, r4, lo(DEV_UART0)
endfunc early_console_putc

func early_console_puts
	l.sw	-4(r1), r2
	l.sw	-8(r1), r9
	l.addi	r1, r1, -8
	l.ori	r2, r3, 0
1:	l.lbz	r3, 0(r2)
	l.sfeqi	r3, 0
	l.bf	2f
	l.nop
	l.jal	early_console_putc
	l.addi	r2, r2, 1
	l.j	1b
2:	l.lwz	r9, 0(r1)
	l.addi	r1, r1, 8
	l.jr	r9
	l.lwz	r2, -4(r1)
endfunc early_console_puts
