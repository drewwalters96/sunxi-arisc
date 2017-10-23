/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DRIVERS_IRQCHIP_H
#define DRIVERS_IRQCHIP_H

#include <device.h>

void irqchip_irq(void);
int irqchip_register_irq(struct device *dev, void (*handler)(struct device *));
int irqchip_unregister_irq(struct device *dev);

#endif /* DRIVERS_IRQCHIP_H */
