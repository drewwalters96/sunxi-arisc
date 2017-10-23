/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DRIVERS_CLOCK_H
#define DRIVERS_CLOCK_H

enum clock_type {
	CLOCK_TYPE_NONE = 0,
};

struct clock_bus {
	const char *name;
};

struct clock_device {
	struct clock_bus *parent;
	uint8_t           gate;
	uint8_t           reset;
	uint8_t           type;
};

int clock_disable_unprepare(struct clock_device *clk);
int clock_prepare_enable(struct clock_device *clk);

#endif /* DRIVERS_CLOCK_H */
