/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <types.h>

#define DEVICE_STATE_NEW      0
#define DEVICE_STATE_RUNNING  1
#define DEVICE_STATE_DISABLED 2
#define DEVICE_STATE_ERROR    3

#define __device              __attribute__((section(".device"), used))
#define __driver              __attribute__((section(".driver"), used))

struct driver;

struct device {
	const char    *name;
	uintptr_t      base;
	uintptr_t      clock;
	uintptr_t      data;
	struct driver *drv;
	struct device *parent;
	uint8_t        class;
	uint8_t        state;
};

struct driver {
	const char *name;
	int         (*init)(void);
	int         (*probe)(struct device *dev);
	int         (*remove)(struct device *dev);
};

int device_model_init(void);
int device_probe(struct device *dev);
int device_remove(struct device *dev);

#endif /* DEVICE_H */
