/*
 * arch/arm/mach-spear13xx/localtimer.c
 * Directly picked from realview
 *
 * Copyright (C) 2010 ST Microelectronics Ltd.
 * Shiraz Hashim <shiraz.hashim@st.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/smp.h>
#include <linux/clockchips.h>
#include <asm/irq.h>
#include <asm/smp_twd.h>
#include <asm/localtimer.h>

/* Setup the local clock events for a CPU. */
void __cpuinit local_timer_setup(struct clock_event_device *evt)
{
	evt->irq = IRQ_LOCALTIMER;
	twd_timer_setup(evt);
}
