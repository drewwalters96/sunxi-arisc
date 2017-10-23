/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <device.h>
#include <error.h>
#include <limits.h>

extern struct device __device_list[];
extern struct device __device_list_end[];
extern struct driver __driver_list[];
extern struct driver __driver_list_end[];

int
device_probe(struct device *dev)
{
	int err;

	assert(dev->drv);
	assert(dev->drv->probe);

	switch (dev->state) {
	case DEVICE_STATE_NEW:
		break;
	case DEVICE_STATE_RUNNING:
		return SUCCESS;
	case DEVICE_STATE_DISABLED:
		break;
	case DEVICE_STATE_ERROR:
		break;
	case DEVICE_STATE_MISSING:
		return ENODEV;
	}
	if (dev->parent && (err = device_probe(dev->parent))) {
		/* Only copy the error from the parent when it applies here. */
		if (err != ENODEV)
			err = ENOBUS;
		goto fail;
	}
	if ((err = dev->drv->probe(dev)))
		goto fail;

	dev->state = DEVICE_STATE_RUNNING;
	return SUCCESS;

fail:
	if (err == ENODEV) {
		warn("tried to probe missing device %s", dev->name);
		dev->state = DEVICE_STATE_MISSING;
	} else {
		error("failed to probe device %s (%d)", dev->name, err);
		dev->state = DEVICE_STATE_ERROR;
	}

	return err;
}

int
device_remove(struct device *dev)
{
	struct device *child;
	int err;

	assert(dev->drv);
	assert(dev->drv->remove);

	switch (dev->state) {
	case DEVICE_STATE_NEW:
		dev->state = DEVICE_STATE_DISABLED;
		return SUCCESS;
	case DEVICE_STATE_RUNNING:
		break;
	case DEVICE_STATE_DISABLED:
		return SUCCESS;
	case DEVICE_STATE_ERROR:
		warn("tried to remove failed device %s", dev->name);
		return SUCCESS;
	case DEVICE_STATE_MISSING:
		return SUCCESS;
	}
	for (child = __device_list; child < __device_list_end; ++child) {
		if (child->parent == dev &&
		    child->state == DEVICE_STATE_RUNNING)
			return EBUSY;
	}
	if ((err = dev->drv->remove(dev))) {
		error("failed to remove device %s (%d)", dev->name, err);
		dev->state = DEVICE_STATE_ERROR;
		return err;
	}

	dev->state = DEVICE_STATE_DISABLED;
	return SUCCESS;
}

void
driver_model_exit(void)
{
	struct device *dev;
	int parents = INT_MAX;
	int previous;

	do {
		previous = parents;
		parents  = 0;
		for (dev = __device_list; dev < __device_list_end; ++dev) {
			if (dev->flags & DEVICE_FLAG_CRITICAL)
				continue;
			if (device_remove(dev) == EBUSY)
				++parents;
		}
		debug("removing non-critical devices, %d remaining", parents);
	} while (parents && parents < previous);
	debug("%d critical devices not removed", parents);
}

void
driver_model_init(void)
{
	struct device *dev;
	struct driver *drv;
	int err;

	for (drv = __driver_list; drv < __driver_list_end; ++drv)
		if (drv->init && (err = drv->init()))
			panic("failed to initialize driver %s (%d)", drv->name,
			      err);
	for (dev = __device_list; dev < __device_list_end; ++dev) {
		if ((err = device_probe(dev)) && err != ENODEV) {
			driver_model_exit();
			panic("failed to initialize devices");
		}
	}
}
