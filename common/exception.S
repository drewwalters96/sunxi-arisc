/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <assembler.S>
#include <arch/exception.h>

	/* This macro is used to create an exception vector. */
	.macro vector num label
	.section .vectors, "ax", @progbits
	.org EXCEPTION_VECTOR_ADDRESS(\num)
	.global exception_vector_\num
	.type exception_vector_\num, %function
	.func exception_vector_\num
	.cfi_sections .debug_frame
	.cfi_startproc
exception_vector_\num:
	l.j \label
	l.nop
	.cfi_endproc
	.endfunc
	.size exception_vector_\num, . - exception_vector_\num
	.endm

vector RESET_EXCEPTION start
