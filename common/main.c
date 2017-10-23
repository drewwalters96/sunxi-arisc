/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <console.h>
#include <debug.h>
#include <device.h>
#include <platform/devices.h>

void
main(void)
{
	console_init(DEV_UART0);
	driver_model_init();
	info("Hello, world!");
}
