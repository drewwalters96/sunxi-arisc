/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <console.h>
#include <stdio.h>

void
putc(uint32_t c)
{
	if (c == '\n')
		console_putc('\r');
	console_putc(c);
}
