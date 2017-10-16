/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <assembler.S>

func do_panic
1:	l.j	1b
	l.nop
endfunc do_panic
