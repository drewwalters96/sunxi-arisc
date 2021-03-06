/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: 0BSD
 */

#include <assembler.S>

func __bswapsi2
	l.addi	r6, r0, 0xff
	l.and	r4, r3, r6
	l.srli	r3, r3, 8
	l.slli	r4, r4, 8
	l.and	r5, r3, r6
	l.or	r4, r4, r5
	l.srli	r3, r3, 8
	l.slli	r4, r4, 8
	l.and	r5, r3, r6
	l.or	r4, r4, r5
	l.srli	r3, r3, 8
	l.slli	r4, r4, 8
	l.and	r5, r3, r6
	l.jr	r9
	l.or	r11, r4, r5
endfunc __bswapsi2

func __ctzi2
	l.fl1	r11, r3
	l.jr	r9
	l.addi	r11, r11, -1
endfunc __ctzi2

/*
 * Optimized implementation of the "shift divisor method" algorithm from
 * T. Rodeheffer. Software Integer Division. Microsoft Research, 2008.
 */
func __udivsi3				# u32 __udivsi3(u32 x, u32 y) {
	l.ori	r5, r3, 0		#	u32 r = x;
	l.ori	r6, r4, 0		#	u32 y0 = y;
	l.addi	r11, r0, 0		#	u32 q = 0;
	l.sfltu	r3, r4			#	if (x >= y) {
	l.bf	2f
	l.sub	r3, r3, r4		# 		x = x−y;
1:	l.sfltu	r3, r4			#		while (x >= y) {
	l.bf	2f
	l.sub	r3, r3, r4		#			x = x−y;
	l.add	r4, r4, r4		#			y *= 2;
	l.j	1b			#		}
2:	l.sfltu	r5, r4			# 	} for (;;) {
	l.bf	3f			#		if (r >= y) {
	l.sfeq	r4, r6			#		[if (y == y0)]
	l.sub	r5, r5, r4		#			r = r−y;
	l.addi	r11, r11, 1		#			q = q + 1;
3:	l.bf	4f			#		} if (y == y0) break;
	l.srli	r4, r4, 1		#		y >>= 1;
	l.j	2b			#	}
	l.add	r11, r11, r11		#		q *= 2;
4:	l.jr	r9			#	return q;
	l.nop				# }
endfunc __udivsi3
