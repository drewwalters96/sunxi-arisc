/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <dm.h>
#include <error.h>
#include <mmio.h>
#include <util.h>
#include <drivers/clock.h>
#include <drivers/sunxi-ccu.h>
#include <platform/ccu.h>

static int
sunxi_ccu_probe(struct device *dev)
{
	return SUCCESS;
}

static int
sunxi_ccu_disable(struct device *clockdev, struct device *dev)
{
	uint8_t gate;
	uint8_t reset;

	if ((reset = CCU_GET_RESET(dev->clock))) {
		mmio_clearbits32(clockdev->address + CCU_RESET_BASE +
		                 BITFIELD_WORD(reset),
		                 BIT(BITFIELD_BIT(reset)));
	}
	if ((gate = CCU_GET_GATE(dev->clock))) {
		mmio_clearbits32(clockdev->address + CCU_GATE_BASE +
		                 BITFIELD_WORD(gate),
		                 BIT(BITFIELD_BIT(gate)));
	}

	return SUCCESS;
}

static int
sunxi_ccu_enable(struct device *clockdev, struct device *dev)
{
	uint8_t gate;
	uint8_t reset;

	if ((gate = CCU_GET_GATE(dev->clock))) {
		mmio_setbits32(clockdev->address + CCU_GATE_BASE +
		               BITFIELD_WORD(gate),
		               BIT(BITFIELD_BIT(gate)));
	}
	if ((reset = CCU_GET_RESET(dev->clock))) {
		mmio_setbits32(clockdev->address + CCU_RESET_BASE +
		               BITFIELD_WORD(reset),
		               BIT(BITFIELD_BIT(reset)));
	}

	return SUCCESS;
}

static int
sunxi_ccu_set_freq(struct device *clockdev, struct device *dev, uint32_t hz)
{
	return ENOTSUP;
}

static struct clock_driver_ops sunxi_ccu_driver_ops = {
	.class    = DM_CLASS_CLOCK,
	.disable  = sunxi_ccu_disable,
	.enable   = sunxi_ccu_enable,
	.set_freq = sunxi_ccu_set_freq,
};

struct driver sunxi_ccu_driver __driver = {
	.name  = "sunxi-ccu",
	.probe = sunxi_ccu_probe,
	.ops   = &sunxi_ccu_driver_ops,
};
