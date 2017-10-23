/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <device.h>
#include <error.h>
#include <mmio.h>
#include <util.h>
#include <drivers/clock.h>
#include <drivers/irqchip.h>
#include <platform/ccu.h>
#include <platform/devices.h>
#include <platform/irq.h>

#define MSGBOX_QUEUES             8

#define MSGBOX_USER_ARISC         0
#define MSGBOX_USER_ARM           1

#define MSGBOX_CTRL_RX(n, u)      (BIT(8 * ((n) % 4)) * (u))
#define MSGBOX_CTRL_TX(n, u)      (BIT(8 * ((n) % 4) + 4) * (u))
#define MSGBOX_SENDER(n, u)       (MSGBOX_CTRL_RX(n, !(u)) | \
	                           MSGBOX_CTRL_TX(n, u))

#define MSGBOX_IRQ_BOTH(n)        BITMASK(2 * (n), 2)
#define MSGBOX_IRQ_RX(n)          BIT(2 * (n))
#define MSGBOX_IRQ_TX(n)          BIT(2 * (n) + 1)

#define MSGBOX_MSG_STATUS_MSG_NUM BITMASK(0, 3)

#define MSGBOX_CTRL_REG0          (0x0000)
#define MSGBOX_CTRL_REG1          (0x0004)
#define MSGBOX_IRQ_EN_REG(u)      (0x0040 + 0x20 * (u))
#define MSGBOX_IRQ_STATUS_REG(u)  (0x0050 + 0x20 * (u))
#define MSGBOX_FIFO_STATUS_REG(n) (0x0100 + 0x04 * (n))
#define MSGBOX_MSG_STATUS_REG(n)  (0x0140 + 0x04 * (n))
#define MSGBOX_MSG_REG(n)         (0x0180 + 0x04 * (n))

#define ME                        MSGBOX_USER_ARISC

static void msgbox_handle_msg(struct device *dev, uint32_t queue);
static void msgbox_irq(struct device *dev);
static int msgbox_probe(struct device *dev);
static int msgbox_remove(struct device *dev);

static struct driver msgbox_driver __driver = {
	.name   = "msgbox",
	.probe  = msgbox_probe,
	.remove = msgbox_remove,
};

static struct device msgbox_devices[] __device = {
	{
		.name  = "msgbox",
		.drv   = &msgbox_driver,
		.base  = DEV_MSGBOX,
		.clock ={
			.gate  = CCU_GATE_MSGBOX,
			.reset = CCU_RESET_MSGBOX,
		},
		.irq = IRQ_MSGBOX,
	},
};

static void
msgbox_handle_msg(struct device *dev, uint32_t queue)
{
	info("msgbox received message %08x in queue %d",
	     mmio_read32(dev->base + MSGBOX_MSG_REG(queue)), queue);
}

static void
msgbox_irq(struct device *dev)
{
	uint32_t status = mmio_read32(dev->base + MSGBOX_IRQ_STATUS_REG(ME));

	for (uint32_t queue = 0; queue < MSGBOX_QUEUES; ++queue) {
		if (!(status & MSGBOX_IRQ_RX(queue)))
			continue;
		while (mmio_read32(dev->base + MSGBOX_MSG_STATUS_REG(queue)) &
		       MSGBOX_MSG_STATUS_MSG_NUM)
			msgbox_handle_msg(dev, queue);
	}

	/* Clear all processed pending interrupts. */
	mmio_write32(dev->base + MSGBOX_IRQ_STATUS_REG(ME), status);
}

static int
msgbox_probe(struct device *dev)
{
	int err;

	if ((err = clock_prepare_enable(&dev->clock)))
		return err;

	/* Set queue directions. */
	mmio_write32(dev->base + MSGBOX_CTRL_REG0,
	             MSGBOX_SENDER(0, MSGBOX_USER_ARM) |
	             MSGBOX_SENDER(1, MSGBOX_USER_ARM) |
	             MSGBOX_SENDER(2, MSGBOX_USER_ARM) |
	             MSGBOX_SENDER(3, MSGBOX_USER_ARM));
	mmio_write32(dev->base + MSGBOX_CTRL_REG1,
	             MSGBOX_SENDER(4, MSGBOX_USER_ARM) |
	             MSGBOX_SENDER(5, MSGBOX_USER_ARM) |
	             MSGBOX_SENDER(6, MSGBOX_USER_ARM) |
	             MSGBOX_SENDER(7, MSGBOX_USER_ARM));

	/* Enable and clear IRQs. */
	mmio_write32(dev->base + MSGBOX_IRQ_STATUS_REG(ME),
	             MSGBOX_IRQ_BOTH(0) | MSGBOX_IRQ_BOTH(1) | \
	             MSGBOX_IRQ_BOTH(2) | MSGBOX_IRQ_BOTH(3) | \
	             MSGBOX_IRQ_BOTH(4) | MSGBOX_IRQ_BOTH(5) | \
	             MSGBOX_IRQ_BOTH(6) | MSGBOX_IRQ_BOTH(7));
	mmio_write32(dev->base + MSGBOX_IRQ_EN_REG(ME),
	             MSGBOX_IRQ_RX(0) | MSGBOX_IRQ_RX(1) | \
	             MSGBOX_IRQ_RX(2) | MSGBOX_IRQ_RX(3) | \
	             MSGBOX_IRQ_RX(4) | MSGBOX_IRQ_RX(5) | \
	             MSGBOX_IRQ_RX(6) | MSGBOX_IRQ_RX(7));

	if ((err = irqchip_register_irq(dev, msgbox_irq)))
		return err;

	return SUCCESS;
}

static int
msgbox_remove(struct device *dev)
{
	int err;

	/* Disable IRQs. */
	mmio_write32(dev->base + MSGBOX_IRQ_EN_REG(ME), 0);

	if ((err = irqchip_unregister_irq(dev)))
		return err;
	if ((err = clock_disable_unprepare(&dev->clock)))
		return err;

	return SUCCESS;
}
