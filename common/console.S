/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <assembler.S>
#include <util.h>

#define UART_THR			0x0000
#define UART_LSR			0x0014
#define UART_LSR_THRE			BIT(5)

data console_base
	.word	0

func console_init
	l.movhi	r4, hi(console_base)
	l.ori	r4, r4, lo(console_base)
	l.jr	r9
	l.sw	0(r4), r3
endfunc console_init

func console_putc
	l.movhi	r31, hi(console_base)
	l.ori	r31, r31, lo(console_base)
	l.lwz	r29, 0(r31)
	l.sfeq	r29, r0
	l.bf	2f
	l.lwz	r31, UART_LSR(r29)
1:	l.andi	r31, r31, UART_LSR_THRE
	l.sfeq	r31, r0
	l.bf	1b
	l.lwz	r31, UART_LSR(r29)
	l.andi	r3, r3, 0xff
	l.sw	UART_THR(r29), r3
2:	l.jr	r9
	l.nop
endfunc console_putc
