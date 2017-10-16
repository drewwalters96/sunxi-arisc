/*
 * Copyright © 2005-2014 Rich Felker, et al.
 * SPDX-License-Identifier: MIT
 */

#ifndef CTYPE_H
#define CTYPE_H

#include <compiler.h>

#define isalpha(a) ((((unsigned int) (a)) | 32) - 'a' < 26)
#define isascii(a) (((unsigned int) (a)) < 128)
#define isdigit(a) (((unsigned int) (a)) - '0' < 10)
#define isgraph(a) (((unsigned int) (a)) - 0x21 < 0x5e)
#define islower(a) (((unsigned int) (a)) - 'a' < 26)
#define isprint(a) (((unsigned int) (a)) - 0x20 < 0x5f)
#define isupper(a) (((unsigned int) (a)) - 'A' < 26)
#define tolower(a) ((a) | 0x20)
#define toupper(a) ((a) & 0x5f)

static inline bool
isalnum(char c)
{
	return isalpha(c) || isdigit(c);
}

static inline bool
isblank(char c)
{
	return c == ' ' || c == '\t';
}

static inline bool
iscntrl(char c)
{
	return ((unsigned int) c) < 0x20 || c == 0x7f;
}

static inline bool
ispunct(char c)
{
	return isgraph(c) && !isalnum(c);
}

static inline bool
isspace(char c)
{
	return c == ' ' || ((unsigned int) c) - '\t' < 5;
}

static inline bool
isxdigit(char c)
{
	return isdigit(c) || (((unsigned int) c) | 32) - 'a' < 6;
}

#endif /* CTYPE_H */
