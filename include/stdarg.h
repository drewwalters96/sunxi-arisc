/*
 * Copyright © 2005-2014 Rich Felker, et al.
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef STDARG_H
#define STDARG_H

#define va_arg(v, l)   __builtin_va_arg(v, l)
#define va_copy(d, s)  __builtin_va_copy(d, s)
#define va_end(v)      __builtin_va_end(v)
#define va_start(v, l) __builtin_va_start(v, l)

typedef __builtin_va_list va_list;

#endif /* STDARG_H */
