/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <types.h>

void console_init(uintptr_t base);

/*
 * Write a single character to the console.
 *
 * This function only clobbers r3, r29, and r31.
 */
void console_putc(uint32_t c);

#endif /* CONSOLE_H */
