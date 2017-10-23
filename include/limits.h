/*
 * Copyright © 2005-2014 Rich Felker, et al.
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef LIMITS_H
#define LIMITS_H

#define CHAR_BIT    8
#define CHAR_MAX    0xff
#define CHAR_MIN    0

#define INT8_MAX    0x7f
#define INT8_MIN    (-1 - 0x7f)
#define INT16_MAX   0x7fff
#define INT16_MIN   (-1 - 0x7fff)
#define INT32_MAX   0x7fffffff
#define INT32_MIN   (-1 - 0x7fffffff)
#define INT64_MAX   0x7fffffffffffffff
#define INT64_MIN   (-1 - 0x7fffffffffffffff)

#define INT_MAX     INT32_MAX
#define INT_MIN     INT32_MIN
#define INTMAX_MAX  INT64_MAX
#define INTMAX_MIN  INT64_MIN
#define INTPTR_MAX  INT32_MAX
#define INTPTR_MIN  INT32_MIN

#define LONG_MAX    0x7fffffffL
#define LONG_MIN    (-0x7fffffffL - 1)
#define LLONG_MAX   0x7fffffffffffffffLL
#define LLONG_MIN   (-0x7fffffffffffffffLL - 1)

#define PTRDIFF_MAX INT32_MAX
#define PTRDIFF_MIN INT32_MIN

#define SCHAR_MAX   0x7f
#define SCHAR_MIN   (-1 - 0x7f)

#define SHRT_MAX    0x7fff
#define SHRT_MIN    (-1 - 0x7fff)

#define SIZE_MAX    UINT32_MAX

#define UCHAR_MAX   0xff

#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffffU
#define UINT64_MAX  0xffffffffffffffffU

#define UINT_MAX    UINT32_MAX
#define UINTMAX_MAX UINT64_MAX
#define UINTPTR_MAX UINT32_MAX

#define ULONG_MAX   0xffffffffUL
#define ULLONG_MAX  0xffffffffffffffffULL

#define USHRT_MAX   0xffff

#define WORDSIZE    32

#endif /* LIMITS_H */
