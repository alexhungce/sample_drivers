/*
 *  led sample driver
 *
 *  Copyright (C) 2017 Alex Hung <alex.hung@canonical.com>
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
#include <linux/platform_device.h>
#include <linux/leds.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");

static struct platform_device *led_pl_dev;

static void led_sample_set(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	printk("led_sample_set: %d\n", value);
}

static struct led_classdev sample_led = {
	.name = "led::sample",
	.brightness_set = led_sample_set,
	.max_brightness = 1,
	.flags = LED_CORE_SUSPENDRESUME,
	.default_trigger = "rfkill-any", /* requires kernel 4.11 or later */
};

static int led_init(struct device *dev)
{
	return devm_led_classdev_register(dev, &sample_led);
}

static int led_platform_probe(struct platform_device *device)
{
	int err;

	err = led_init(&device->dev);

	return err;
}

static int led_platform_remove(struct platform_device *device)
{
	return 0;
}

static struct platform_driver led_pl_driver = {
	.driver = {
		.name = "led-sample",
	},
        .probe = led_platform_probe,
        .remove = led_platform_remove,
};

static int __init led_sample_init(void)
{
	int err;

	printk("led_sample_init\n");

	err = platform_driver_register(&led_pl_driver);
	if (err)
		goto err_driver_reg;

	led_pl_dev = platform_device_alloc("led-sample", -1);
	if (!led_pl_dev) {
		err = -ENOMEM;
		goto err_device_alloc;
	}
	err = platform_device_add(led_pl_dev);
	if (err)
		goto err_device_add;

	return 0;

 err_device_add:
	platform_device_put(led_pl_dev);
 err_device_alloc:
	platform_driver_unregister(&led_pl_driver);
 err_driver_reg:
	return err;
}

static void __exit led_sample_exit(void)
{
	printk("led_sample_exit\n");

	if (led_pl_dev) {
		platform_device_unregister(led_pl_dev);
		platform_driver_unregister(&led_pl_driver);
	}
}

module_init(led_sample_init);
module_exit(led_sample_exit);
