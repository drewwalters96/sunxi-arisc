/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef BYTESWAP_H
#define BYTESWAP_H

#include <types.h>

static inline uint16_t
bswap16(uint16_t n)
{
	return ((n << 8)  & 0xff00U) | \
	       ((n >> 8)  & 0xffU);
}

static inline uint32_t
bswap32(uint32_t n)
{
	return ((n << 24) & 0xff000000U) | \
	       ((n << 8)  & 0xff0000U) | \
	       ((n >> 8)  & 0xff00U) | \
	       ((n >> 24) & 0xffU);
}

#endif /* BYTESWAP_H */
