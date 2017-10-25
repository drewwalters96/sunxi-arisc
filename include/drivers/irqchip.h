/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DRIVERS_IRQCHIP_H
#define DRIVERS_IRQCHIP_H

#include <dm.h>
#include <types.h>

#define IRQCHIP_OPS(ops) ((struct irqchip_driver_ops *)(ops))

struct irqchip_driver_ops {
	uint32_t class;
	int      (*irq)(struct device *irqdev);
	int      (*register_irq)(struct device *irqdev, struct device *dev,
	                         void (*handler)(struct device *));
	int      (*unregister_irq)(struct device *irqdev, struct device *dev);
};

int irqchip_device_register(struct device *dev, uint8_t channel);
int irqchip_irq(void);

static inline int
irqchip_register_irq(struct device *dev, void (*handler)(struct device *))
{
	struct device *irqdev = dev->irqdev;

	return IRQCHIP_OPS(irqdev->drv->ops)->register_irq(irqdev, dev,
	                                                   handler);
}

static inline int
irqchip_unregister_irq(struct device *dev)
{
	struct device *irqdev = dev->irqdev;

	return IRQCHIP_OPS(irqdev->drv->ops)->unregister_irq(irqdev, dev);
}

#endif /* DRIVERS_IRQCHIP_H */
