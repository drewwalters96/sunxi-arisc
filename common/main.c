/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <dm.h>
#include <stdio.h>
#include <drivers/console.h>

void
main(void)
{
	early_console_init();
	dm_init();
	info("Hello, world!");
}
