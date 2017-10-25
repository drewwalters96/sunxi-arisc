/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <dm.h>
#include <error.h>
#include <arch/spr.h>
#include <drivers/irqchip.h>

#define PIC_CHANNELS 1

static struct device *irqchip_devices[PIC_CHANNELS];

int
irqchip_device_register(struct device *dev, uint8_t channel)
{
	/* fixme: use only channels of enabled irq chips, need to dump these to
	 * figure out where intc is connected */
	assert(channel < PIC_CHANNELS);

	irqchip_devices[channel] = dev;

	/* Enable the CPU external interrupt input. */
	mtspr(SPR_SYS_SR_ADDR, SPR_SYS_SR_IEE_SET(mfspr(SPR_SYS_SR_ADDR), 1));
	debug("interrupts are %s",
	      SPR_SYS_SR_IEE_GET(mfspr(SPR_SYS_SR_ADDR)) ?
	      "enabled" : "disabled");

	return SUCCESS;
}

int
irqchip_irq(void)
{
	/* figure out which irqchip this goes to */
	struct device *irqdev;
	uint32_t channel = 0;

	if (!(irqdev = irqchip_devices[channel]))
		panic("no irqchip for interrupt on channel %d", channel);

	return IRQCHIP_OPS(irqdev->drv->ops)->irq(irqchip_devices[channel]);
}
