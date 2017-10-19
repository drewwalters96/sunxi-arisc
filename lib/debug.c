/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <compiler.h>
#include <ctype.h>
#include <debug.h>
#include <stdarg.h>
#include <stdio.h>
#include <types.h>

#define BYTES_PER_ROW			16
#define BYTES_PER_WORD			sizeof(uint32_t)

void
hexdump(uintptr_t addr, uint32_t bytes)
{
	uintptr_t start;

	addr &= ~(BYTES_PER_ROW - 1);
	for (start = addr; addr - start < bytes; addr += BYTES_PER_ROW) {
		uint32_t *words = (uint32_t *) addr;
		printf("%08x: %08x %08x %08x %08x  ",
		       addr, words[0], words[1], words[2], words[3]);
		/* The ARISC processor's data lines are swapped in hardware for
		 * compatibility with the little-endian ARM CPUs. To examine
		 * individual bytes, we must reverse each group of 4 bytes. */
		for (uint32_t i = BYTES_PER_WORD - 1; i < BYTES_PER_ROW; --i) {
			char c = ((char *) addr)[i];
			putc(isprint(c) ? c : '.');
			if (i % BYTES_PER_WORD == 0)
				i += 2 * BYTES_PER_WORD;
		}
		putc('\n');
	}
}

void __printf(2, 3)
log(unsigned int level, const char *fmt, ...)
{
	const char *prefix;
	va_list args;

	assert(level <= (DEBUG ? LOG_LEVEL_DEBUG : LOG_LEVEL_INFO));
	switch (level) {
		case LOG_LEVEL_FATAL:
			prefix = "FATAL:\t";
			break;
		case LOG_LEVEL_ERROR:
			prefix = "ERROR:\t";
			break;
		case LOG_LEVEL_WARNING:
			prefix = "WARNING:";
			break;
		case LOG_LEVEL_INFO:
			prefix = "INFO:\t";
			break;
		case LOG_LEVEL_DEBUG:
			prefix = "DEBUG:\t";
			break;
		default:
			unreachable();
	}
	puts(prefix);
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	putc('\n');
}
