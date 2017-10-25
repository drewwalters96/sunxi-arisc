/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <assembler.S>
#include <util.h>

#define UART_THR			0x0000
#define UART_LSR			0x0014
#define UART_LSR_THRE			BIT(5)

func early_uart_putc
	l.andi	r3, r3, 0xff
	l.sfeqi	r3, '\n'
	l.bnf	1f
	l.cmov	r6, r3, r0
	l.ori	r3, r0, '\r'
1:	l.lwz	r5, UART_LSR(r4)
2:	l.andi	r5, r5, UART_LSR_THRE
	l.sfeqi	r5, 0
	l.bf	2b
	l.lwz	r5, UART_LSR(r4)
	l.sw	UART_THR(r4), r3
	l.ori	r3, r6, 0
	l.sfnei	r3, 0
	l.bf	1b
	l.addi	r6, r0, 0
	l.jr	r9
	l.nop
endfunc early_uart_putc
