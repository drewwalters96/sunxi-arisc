/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <debug.h>
#include <exception.h>
#include <types.h>
#include <arch/exception.h>
#include <drivers/irqchip.h>

void
exception_handler(uint32_t number, struct exception_regs *regs)
{
	switch (number) {
	case TICK_TIMER_EXCEPTION:
		break;
	case EXTERNAL_INTERRUPT:
		irqchip_irq();
		break;
	default:
		panic("unhandled exception %d at %p! (lr=%p)",
		      number, (void *)regs->pc, (void *)regs->r9);
	}
}
