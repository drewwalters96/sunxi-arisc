/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <dm.h>
#include <error.h>
#include <drivers/console.h>

static struct device *console_device;

int
console_device_register(struct device *dev)
{
	if (console_device)
		return EEXIST;
	console_device = dev;

	return SUCCESS;
}

void
putc(uint32_t c)
{
	if (likely(console_device))
		CONSOLE_OPS(console_device->drv->ops)->putc(console_device, c);
	else
		early_console_putc(c);
}

void
puts(const char *s)
{
	if (likely(console_device))
		CONSOLE_OPS(console_device->drv->ops)->puts(console_device, s);
	else
		early_console_puts(s);
}
