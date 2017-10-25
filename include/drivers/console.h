/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DRIVERS_CONSOLE_H
#define DRIVERS_CONSOLE_H

#include <dm.h>
#include <types.h>

#define CONSOLE_OPS(ops) ((struct console_driver_ops *)(ops))

struct console_driver_ops {
	uint32_t class;
	void     (*putc)(struct device *dev, uint32_t c);
	void     (*puts)(struct device *dev, const char *s);
};

int console_device_register(struct device *dev);
void early_console_init(void);
void early_console_putc(uint32_t c);
void early_console_puts(const char *s);
void early_uart_putc(uint32_t c, uintptr_t uart_base);

#endif /* DRIVERS_CONSOLE_H */
