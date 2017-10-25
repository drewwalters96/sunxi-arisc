/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DRIVERS_MSGBOX_H
#define DRIVERS_MSGBOX_H

#include <dm.h>
#include <types.h>

struct msgbox_driver_ops {
	uint32_t class;
};

#endif /* DRIVERS_MSGBOX_H */
