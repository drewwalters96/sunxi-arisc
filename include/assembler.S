/*
 * Copyright © 2013-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ASSEMBLER_S
#define ASSEMBLER_S

	/* This macro is used to mark the beginning of a function. */
	.macro func name
	.section .text.\name, "ax", @progbits
	.global \name
	.type \name, %function
	.func \name
	.cfi_sections .debug_frame
	.cfi_startproc
	.align 4
\name:
	.endm

	/* This macro is used to mark the end of a function. */
	.macro endfunc name
	.cfi_endproc
	.endfunc
	.size \name, . - \name
	.endm

#endif /* ASSEMBLER_S */
