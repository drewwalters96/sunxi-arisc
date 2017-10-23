/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <device.h>
#include <error.h>
#include <mmio.h>
#include <util.h>
#include <arch/spr.h>
#include <drivers/irqchip.h>
#include <platform/devices.h>

#define INTC_VECTOR_REG    0x0000
#define INTC_BASE_ADDR_REG 0x0004
#define INTC_PROTECT_REG   0x0008
#define INTC_NMI_CTRL_REG  0x000c
#define INTC_IRQ_PEND_REG  0x0010
#define INTC_EN_REG        0x0040
#define INTC_MASK_REG      0x0050
#define INTC_RESP_REG      0x0060

#define IRQCHIP_IRQS       32

static int irqchip_init(void);
static int irqchip_probe(struct device *dev);
static int irqchip_remove(struct device *dev);

static struct device *default_irqchip;
static void (*irq_vectors[IRQCHIP_IRQS])(struct device *);
static struct device *irq_devices[IRQCHIP_IRQS];

static struct driver irqchip_driver __driver = {
	.name   = "irqchip",
	.init   = irqchip_init,
	.probe  = irqchip_probe,
	.remove = irqchip_remove,
};

static struct device irqchip_devices[] __device = {
	{
		.name = "irqchip",
		.drv  = &irqchip_driver,
		.base = DEV_R_INTC,
	},
};

static int
irqchip_init(void)
{
	int err;

	/* IRQ chips must be probed early. */
	for (uint32_t i = 0; i < ARRAY_SIZE(irqchip_devices); ++i)
		if ((err = device_probe(&irqchip_devices[i])))
			return err;

	/* Unmask IRQs at the CPU PIC. */
	mtspr(SPR_PIC_PICMR_ADDR, ~0);

	/* Enable the CPU external interrupt input. */
	mtspr(SPR_SYS_SR_ADDR, SPR_SYS_SR_IEE_SET(mfspr(SPR_SYS_SR_ADDR), 1));
	debug("interrupts are %s",
	      SPR_SYS_SR_IEE_GET(mfspr(SPR_SYS_SR_ADDR)) ?
	      "enabled" : "disabled");

	return SUCCESS;
}

void
irqchip_irq(void)
{
	struct device *dev = default_irqchip;
	uint32_t irq;

	/* Get current IRQ. */
	while ((irq = mmio_read32(dev->base + INTC_VECTOR_REG) >> 2)) {
		debug("handling irq %d", irq);

		/* Call registered handler. */
		if (irq_vectors[irq])
			irq_vectors[irq](irq_devices[irq]);
		else
			warn("no ISR registered for IRQ %d", irq);

		/* Clear IRQ pending status. */
		mmio_setbits32(dev->base + INTC_IRQ_PEND_REG, BIT(irq));
	}
}

static int
irqchip_probe(struct device *dev)
{
	/* Clear base address (just return IRQ numbers). */
	mmio_write32(dev->base + INTC_BASE_ADDR_REG, 0);

	/* Disable, unmask, and clear status for all IRQs. */
	mmio_write32(dev->base + INTC_EN_REG, 0);
	mmio_write32(dev->base + INTC_MASK_REG, 0);
	mmio_write32(dev->base + INTC_IRQ_PEND_REG, ~0);

	default_irqchip = dev;

	return SUCCESS;
}

int
irqchip_register_irq(struct device *dev, void (*handler)(struct device *))
{
	uint8_t irq = dev->irq;

	assert(handler);
	assert(!irq_vectors[irq]);

	/* Add IRQ vector. */
	irq_devices[irq] = dev;
	irq_vectors[irq] = handler;

	/* Enable IRQ. */
	mmio_setbits32(default_irqchip->base + INTC_EN_REG, BIT(irq));

	debug("IRQ %d now registered to device %s", irq, dev->name);

	return 0;
}

static int
irqchip_remove(struct device *dev)
{
	if (default_irqchip == dev)
		default_irqchip = NULL;

	return SUCCESS;
}

int
irqchip_unregister_irq(struct device *dev)
{
	uint8_t irq = dev->irq;

	assert(irq_vectors[irq]);

	/* Disable IRQ. */
	mmio_clearbits32(default_irqchip->base + INTC_EN_REG, BIT(irq));

	/* Remove IRQ vector. */
	irq_vectors[irq] = NULL;
	irq_devices[irq] = NULL;

	return 0;
}
