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
#include <linux/kfifo.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");

typedef struct box {
	unsigned int length;
	unsigned int width;
	unsigned int height;
} Box;

static struct kfifo queue;

static int __init queue_init(void)
{
	int ret;
	Box *b1, *b2, *b3;
	printk("queue_init\n");

	ret = kfifo_alloc(&queue, PAGE_SIZE, GFP_KERNEL);
	if (ret)
		return ret;

  b1 = (Box *) kmalloc(sizeof(Box), GFP_KERNEL);
	b1->length = 20;
	b1->width = 10;
	b1->height = 15;

  b2 = (Box *) kmalloc(sizeof(Box), GFP_KERNEL);
	b2->length = 21;
	b2->width = 11;
	b2->height = 16;

  b3 = (Box *) kmalloc(sizeof(Box), GFP_KERNEL);
	b3->length = 22;
	b3->width = 12;
	b3->height = 17;

	if (kfifo_is_empty(&queue))
		printk("queue is empty\n");

	printk("queue size is %u\n", kfifo_size(&queue));

	ret = kfifo_in(&queue, b1, sizeof(Box));
	if (ret != sizeof(Box))
		printk("enqueue is incompleted\n");

	ret = kfifo_in(&queue, b2, sizeof(Box));
	if (ret != sizeof(Box))

	printk("enqueue is incompleted\n");
	ret = kfifo_in(&queue, b3, sizeof(Box));
	if (ret != sizeof(Box))
		printk("enqueue is incompleted\n");

	printk("queue length used is %u\n", kfifo_len(&queue));
	printk("queue size available is %u\n", kfifo_avail(&queue));

	if (kfifo_is_full(&queue))
		printk("queue is full\n");

	kfree(b1);
	kfree(b2);
	kfree(b3);
	return 0;
}

static void __exit queue_exit(void)
{
	int ret, i;
	Box *box = kmalloc(sizeof(Box), GFP_KERNEL);
	printk("queue_exit\n");

	for (i = 0; i < 3; i++) {
		ret = kfifo_out(&queue, box, sizeof(Box));
		if (ret != sizeof(Box)) {
			printk("dequeue is incompleted\n");
			break;
		}

		printk("Box length = %u\n", box->length);
		printk("Box width = %u\n", box->width);
		printk("Box height = %u\n", box->height);

		if (kfifo_is_empty(&queue))
			printk("queue is empty\n");
		else
			printk("queue length used is %u\n", kfifo_len(&queue));
	}

	kfifo_free(&queue);
	kfree(box);

}

module_init(queue_init);
module_exit(queue_exit);
