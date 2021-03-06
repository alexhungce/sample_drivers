/*
 *  Dummy irq handler + work queue
 *
 *  Copyright (C) 2016 Alex Hung <alex.hung@canonical.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");

int irqn = 9;
static unsigned long dummy_dev;

static void workqueue_handler(struct work_struct *work)
{
	printk("workqueue_handler\n");
}

DECLARE_WORK(my_workqueue, workqueue_handler);

static irqreturn_t dummy_isr(int irq, void *dev_id)
{
	printk("dummy_isr: irq = %x\n", irq);
	schedule_work(&my_workqueue);

	/* IRQ_NONE = Not handled, IRQ_HANDLED = handled */
	return IRQ_NONE;
}

static int __init irq_handler_init(void)
{
	printk("irq_handler_init\n");

	/* initialize hardware first */

	/* and then initialize ISR */
	dummy_dev = 0x1025;
	if (request_irq(irqn, dummy_isr, IRQF_SHARED, "dummy", &dummy_dev)) {
		printk(KERN_ERR "irq_handler: fail to register IRQ %d\n", irqn);
		return -EIO;
	}

	return 0;
}

static void __exit irq_handler_exit(void)
{
	printk("irq_handler_exit\n");
	flush_scheduled_work();
	free_irq(irqn, &dummy_dev);
}

module_init(irq_handler_init);
module_exit(irq_handler_exit);
