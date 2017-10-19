/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <device.h>
#include <mmio.h>
#include <types.h>
#include <platform/devices.h>

static int
msgbox_init(void)
{
	return 0;
}

static int
msgbox_probe(struct device *dev)
{
	return 0;
}

static int
msgbox_remove(struct device *dev)
{
	return 0;
}

static struct driver msgbox_driver __driver = {
	.name =		"msgbox",
	.init =		msgbox_init,
	.probe =	msgbox_probe,
	.remove =	msgbox_remove,
};

static struct device msgbox_devices[] __device = {
	{
		.name =		"msgbox",
		.base =		DEV_MSGBOX,
		.drv =		&msgbox_driver,
	},
};
