/*
 * Copyright © 2005-2014 Rich Felker, et al.
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef TYPES_H
#define TYPES_H

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef long long intmax_t;
typedef int intptr_t;

typedef int ptrdiff_t;

typedef unsigned int size_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long long uintmax_t;
typedef unsigned int uintptr_t;

#endif /* TYPES_H */
