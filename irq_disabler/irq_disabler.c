/*
 *  irq disabler
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
#include <linux/interrupt.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");

int irqn = 7;
static struct platform_device *irq_disable_pl_dev;

static int irq_pl_suspend_handler(struct device *device)
{
	printk("irq_pl_suspend_handler\n");
	return 0;
}

static int irq_pl_resume_handler(struct device *device)
{
	printk("irq_pl_resume_handler\n");
	disable_irq_nosync(irqn);
	return 0;
}

static const struct dev_pm_ops irq_pl_pm_ops = {
	.suspend  = irq_pl_suspend_handler,
	.resume   = irq_pl_resume_handler,
};

static struct platform_driver irq_disable_pl_driver = {
	.driver = {
		.name = "irq-disabler",
		.pm = &irq_pl_pm_ops,
	},
};

static int __init irq_disabler_init(void)
{
	int err;

	printk("irq_disabler_init\n");

	disable_irq_nosync(irqn);

	err = platform_driver_register(&irq_disable_pl_driver);
	if (err)
		goto err_driver_reg;

	irq_disable_pl_dev = platform_device_alloc("irq-disabler", -1);
	if (!irq_disable_pl_dev) {
		err = -ENOMEM;
		goto err_device_alloc;
	}
	err = platform_device_add(irq_disable_pl_dev);
	if (err)
		goto err_device_add;

	return 0;

err_device_add:
	platform_device_put(irq_disable_pl_dev);
err_device_alloc:
	platform_driver_unregister(&irq_disable_pl_driver);
err_driver_reg:
	return err;
}

static void __exit irq_disabler_exit(void)
{
	printk("irq_disabler_exit\n");

	enable_irq(irqn);

	if (irq_disable_pl_dev) {
		platform_device_unregister(irq_disable_pl_dev);
		platform_driver_unregister(&irq_disable_pl_driver);
	}
}

module_init(irq_disabler_init);
module_exit(irq_disabler_exit);
