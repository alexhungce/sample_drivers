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
#include <linux/input.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");

static LIST_HEAD(box_list);

typedef struct box {
	unsigned int length;
	unsigned int width;
	unsigned int height;
	struct list_head list;
} Box;

static int __init linkedlist_init(void)
{
	Box *b1, *b2, *b3;
	Box *entry;
	unsigned int count = 0;
	printk("linkedlist_init\n");

  b1 = (Box *) kmalloc(sizeof(Box), GFP_KERNEL);
	b1->length = 20;
	b1->width = 10;
	b1->height = 15;
	INIT_LIST_HEAD(&b1->list);
	list_add(&b1->list, &box_list);

  b2 = (Box *) kmalloc(sizeof(Box), GFP_KERNEL);
	b2->length = 21;
	b2->width = 11;
	b2->height = 16;
	INIT_LIST_HEAD(&b2->list);
	list_add(&b2->list, &box_list);

  b3 = (Box *) kmalloc(sizeof(Box), GFP_KERNEL);
	b3->length = 22;
	b3->width = 12;
	b3->height = 17;
	INIT_LIST_HEAD(&b3->list);
	list_add(&b3->list, &box_list);

	list_for_each_entry(entry, &box_list, list) {
		printk("volume = %d\n", entry->length * entry->width * entry->height);
		count++;
	}

	if (list_empty(&box_list))
		printk("list is empty\n");
	else
		printk("There are %d nodes in box_list\n", count);

	return 0;
}

static void __exit linkedlist_exit(void)
{
	Box *entry, *next;
	printk("linkedlist_exit\n");

	/* remove all nodes */
	list_for_each_entry_safe(entry, next, &box_list, list) {
		list_del(&entry->list);
		kfree(entry);
	}

	if (list_empty(&box_list))
		printk("list is empty\n");
	else
		printk("list is not empty\n");
}

module_init(linkedlist_init);
module_exit(linkedlist_exit);
