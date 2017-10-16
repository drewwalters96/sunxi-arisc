/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <compiler.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <types.h>

static void print_decimal(char sign, int width, bool zero, uint32_t num);
static void print_hex(int width, bool zero, uint32_t num);
static void print_signed(char sign, int width, bool zero, int32_t num);
static void print_unsigned(char sign, int width, bool zero, int32_t num);

static void
fill(int width, bool zero)
{
	for (int i = 0; i < width; ++i)
		putc(zero ? '0' : ' ');
}

static void
print_decimal(char sign, int width, bool zero, uint32_t num)
{
	unsigned digits  = 1;
	unsigned divisor = 1;

	while (divisor <= num / 10) {
		++digits;
		divisor *= 10;
	}
	if (sign) {
		putc(sign);
		--width;
	}
	fill(width - digits, zero);
	while (digits--) {
		uint32_t digit = 0;
		while (num >= divisor) {
			num -= divisor;
			++digit;
		}
		putc(digit + '0');
		divisor /= 10;
	}
}

static void
print_hex(int width, bool zero, uint32_t num)
{
	unsigned bits   = 8 * sizeof(num);
	unsigned digits = 2 * sizeof(num);

	while (digits > 1) {
		if ((num >> (bits - 4)) & 0xf)
			break;
		--digits;
		num <<= 4;
	}
	fill(width - digits, zero);
	while (digits--) {
		uint32_t digit = (num >> (bits - 4)) & 0xf;
		putc(digit < 10 ? digit + '0' : digit - 10 + 'a');
		num <<= 4;
	}
}

static void
print_signed(char sign, int width, bool zero, int32_t num)
{
	if (num < 0)
		print_decimal('-', width, zero, -num);
	else
		print_decimal(sign, width, zero, num);
}

static void
print_unsigned(char sign, int width, bool zero, int32_t num)
{
	print_decimal(sign, width, zero, num);
}

void
printf(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void
puts(const char *s)
{
	char c;

	while ((c = *s++))
		putc(c);
}

void
vprintf(const char *fmt, va_list args)
{
	bool zero;
	char c;
	char sign;
	int  width;
	uintptr_t arg;

	while ((c = *fmt++)) {
		if (c != '%') {
			putc(c);
			continue;
		}
		arg   = va_arg(args, uintptr_t);
		sign  = '\0';
		width = 0;
		zero  = false;
conversion:
		switch (c = *fmt++) {
		case ' ':
			if (!sign)
				sign = ' ';
			goto conversion;
		case '%':
			putc(c);
			break;
		case '+':
			sign = '+';
			goto conversion;
		case 'c':
			fill(width - 1, zero);
			putc(arg);
			break;
		case 'd':
		case 'i':
			print_signed(sign, width, zero, arg);
			break;
		case 'p':
		case 'x':
			print_hex(width, zero, arg);
			break;
		case 's':
			fill(width - strlen((const char *)arg), zero);
			puts((const char *)arg);
			break;
		case 'u':
			print_unsigned(sign, width, zero, arg);
			break;
		default:
			if (c == '0' && width == 0)
				zero = true;
			else if (isdigit(c))
				width = 10 * width + (c - '0');
			goto conversion;
		}
	}
}
