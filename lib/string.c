/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <string.h>
#include <types.h>

size_t
strlen(const char *s)
{
	size_t len = 0;

	while (*s++)
		++len;
	return len;
}
