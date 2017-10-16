/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef STDIO_H
#define STDIO_H

#include <compiler.h>
#include <stdarg.h>
#include <types.h>

void printf(const char *fmt, ...) __printf(1, 2);
void putc(uint32_t c);
void puts(const char *s);
void vprintf(const char *fmt, va_list args) __printf(1, 0);

#endif /* STDIO_H */
