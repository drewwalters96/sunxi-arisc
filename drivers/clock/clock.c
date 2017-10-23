/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <compiler.h>
#include <debug.h>
#include <device.h>
#include <error.h>
#include <mmio.h>
#include <util.h>
#include <drivers/clock.h>
#include <platform/ccu.h>
#include <platform/devices.h>

static int clock_bus_probe(struct device *dev __unused);
static int clock_bus_remove(struct device *dev __unused);

static struct driver clock_bus_driver __driver = {
	.name   = "clock_bus",
	.probe  = clock_bus_probe,
	.remove = clock_bus_remove,
};

static int
clock_bus_probe(struct device *dev __unused)
{
	return SUCCESS;
}

static int
clock_bus_remove(struct device *dev __unused)
{
	return SUCCESS;
}

int
clock_disable_unprepare(struct clock_device *clk)
{
	if (clk->reset) {
		mmio_clearbits32(DEV_CCU + CCU_RESET_BASE +
		                 BITFIELD_WORD(clk->reset),
		                 BIT(BITFIELD_BIT(clk->reset)));
	}
	if (clk->gate) {
		mmio_clearbits32(DEV_CCU + CCU_GATE_BASE +
		                 BITFIELD_WORD(clk->gate),
		                 BIT(BITFIELD_BIT(clk->gate)));
	}

	return SUCCESS;
}

int
clock_prepare_enable(struct clock_device *clk)
{
	if (clk->gate) {
		mmio_setbits32(DEV_CCU + CCU_GATE_BASE +
		               BITFIELD_WORD(clk->gate),
		               BIT(BITFIELD_BIT(clk->gate)));
	}
	if (clk->reset) {
		mmio_setbits32(DEV_CCU + CCU_RESET_BASE +
		               BITFIELD_WORD(clk->reset),
		               BIT(BITFIELD_BIT(clk->reset)));
	}

	return SUCCESS;
}
