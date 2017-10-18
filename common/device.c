/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <device.h>

extern struct device __device_list[];
extern struct device __device_list_end[];
extern struct driver __driver_list[];
extern struct driver __driver_list_end[];

int
device_model_init(void)
{
	struct device *dev;
	struct driver *drv;
	int errors = 0;
	int reason;

	for (drv = __driver_list; drv < __driver_list_end; ++drv)
		if (drv->init && (reason = drv->init()))
			panic("failed to initialize driver %s (%d)", drv->name,
			      reason);
	for (dev = __device_list; dev < __device_list_end; ++dev)
		if (device_probe(dev))
			++errors;
	return errors;
}

int
device_probe(struct device *dev)
{
	int ret = 1;

	assert(dev);
	assert(dev->drv);
	assert(dev->drv->probe);
	if (dev->state == DEVICE_STATE_RUNNING)
		return 0;
	if (dev->state == DEVICE_STATE_ERROR)
		goto fail;
	if (dev->parent && (ret = device_probe(dev->parent)))
		goto fail;
	if ((ret = dev->drv->probe(dev)))
		goto fail;
	dev->state = DEVICE_STATE_RUNNING;
	return 0;

fail:
	error("failed to probe device %s (%d)", dev->name, ret);
	dev->state = DEVICE_STATE_ERROR;
	return ret;
}

int
device_remove(struct device *dev)
{
	struct device *child;
	int ret;

	assert(dev);
	assert(dev->drv);
	assert(dev->drv->remove);
	if (dev->state != DEVICE_STATE_RUNNING)
		return 0;
	for (child = __device_list; child < __device_list_end; ++child)
		if (child->parent == dev)
			return 1;
	if ((ret = dev->drv->remove(dev)))
		goto fail;
	dev->state = DEVICE_STATE_DISABLED;
	return 0;

fail:
	error("failed to remove device %s (%d)", dev->name, ret);
	dev->state = DEVICE_STATE_ERROR;
	return ret;
}
