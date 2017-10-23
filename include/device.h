/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <types.h>
#include <util.h>
#include <drivers/clock.h>

#define __device __attribute__((section(".device"), used))
#define __driver __attribute__((section(".driver"), used))

enum {
	DEVICE_FLAG_CRITICAL = BIT(0),
};

enum {
	DEVICE_STATE_NEW      = 0,
	DEVICE_STATE_RUNNING  = 1,
	DEVICE_STATE_DISABLED = 2,
	DEVICE_STATE_ERROR    = 3,
	DEVICE_STATE_MISSING  = 4,
};

struct driver;

struct device {
	const char         *name;
	uintptr_t           base;
	struct clock_device clock;
	uintptr_t           data;
	struct driver      *drv;
	struct device      *parent;
	uint8_t             flags;
	uint8_t             irq;
	uint8_t             state;
};

struct driver {
	const char *name;
	int         (*init)(void);
	int         (*probe)(struct device *dev);
	int         (*remove)(struct device *dev);
};

int device_probe(struct device *dev);
int device_remove(struct device *dev);
void driver_model_exit(void);
void driver_model_init(void);

#endif /* DEVICE_H */
