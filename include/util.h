/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef UTIL_H
#define UTIL_H

#include <limits.h>

#ifdef __ASSEMBLER__
#define U(n)                      (n)
#else
#define U(n)                      (n ## U)
#endif

#define ARRAY_SIZE(a)             (sizeof(a) / sizeof((a)[0]))

#define BIT(n)                    (U(1) << (n))
#define BITMASK(off, len)         ((BIT(len) - 1) << (off))

#define BITFIELD_INDEX(word, bit) ((word) * WORDSIZE + (bit))
#define BITFIELD_BIT(index)       ((index) % WORDSIZE)
#define BITFIELD_WORD(index)      (((index) / WORDSIZE) * (WORDSIZE / CHAR_BIT))

#ifdef __ASSEMBLER__
#define NULL                      0
#else
#define NULL                      ((void *)0)
#endif

#endif /* UTIL_H */
