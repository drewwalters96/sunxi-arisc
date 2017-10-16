/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <compiler.h>
#include <stdarg.h>
#include <types.h>

#define LOG_LEVEL_FATAL		0
#define LOG_LEVEL_ERROR		1
#define LOG_LEVEL_WARNING	2
#define LOG_LEVEL_INFO		3
#define LOG_LEVEL_DEBUG		4

#if DEBUG
#define assert(e)		((void) ((e) || \
	(panic("\nassertion failed: %s (%s:%d)\n", #e, __FILE__, __LINE__),0)))
#else
#define assert(e)		((void) 0)
#endif

#define panic(...)		(log(LOG_LEVEL_FATAL, __VA_ARGS__), do_panic())
#define error(...)		log(LOG_LEVEL_ERROR, __VA_ARGS__)
#define warn(...)		log(LOG_LEVEL_WARNING, __VA_ARGS__)
#define info(...)		log(LOG_LEVEL_INFO, __VA_ARGS__)
#if DEBUG
#define debug(...)		log(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else /* !DEBUG */
#define debug(...)
#endif /* DEBUG */

noreturn void do_panic(void);
void log(unsigned int level, const char *fmt, ...) __printf(2, 3);

#endif /* DEBUG_H */
