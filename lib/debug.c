/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <compiler.h>
#include <debug.h>
#include <stdarg.h>
#include <stdio.h>

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
