/*
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
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");

void timer_function(unsigned long data)
{
	printk("timer_function: jiffies = %lu, data = 0x%lx", jiffies, data);
}

struct timer_list sample_timer;

static int __init timer_init(void)
{
	init_timer(&sample_timer);
	sample_timer.expires = jiffies + HZ;	/* run after 1 sec */
	sample_timer.data = 0x55aa;
	sample_timer.function = timer_function;
	add_timer(&sample_timer);

	printk("timer_init: jiffies = %lu", jiffies);

	return 0;
}

static void __exit timer_exit(void)
{

}

module_init(timer_init);
module_exit(timer_exit);
