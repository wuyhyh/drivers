/******************************************************************************
 *
 * Name: prim_list.c - Description
 * Created on 2025/03/16
 * Copyright (C) 2022 - 2025, wyh.
 *
 *****************************************************************************/

#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>

#define PRIME_COUNT 100

struct prime_node {
	int prime;
	struct list_head list;
};

static LIST_HEAD(prime_list);

/* 判断质数 */
static bool is_prime(int n)
{
	if (n < 2)
		return false;
	for (int i = 2; i * i <= n; i++) {
		if (n % i == 0)
			return false;
	}
	return true;
}

static struct prime_node *generate_prim_array(int n)
{
	struct prime_node *node = NULL;
	int count = 0;
	int number = 2;

	while (count < n) {
		if (is_prime(number)) {
			node = kmalloc(sizeof(struct prime_node), GFP_KERNEL);
			if (!node) {
				printk(KERN_INFO "kmalloc failed\n");
				return ERR_PTR(-ENOMEM);
			}

			node->prime = number;
			list_add_tail(&node->list, &prime_list);
			count++;
		}
		number++;
	}

	return node;
}

static void print_prim_list(struct prime_node *node)
{
	printk("prime numbers:");
	list_for_each_entry(node, &prime_list, list) {
		printk(" %d", node->prime);
	}
	printk("\n");
}

static int __init prime_list_init(void)
{
	printk(KERN_INFO "Prime-list module loaded\n");

	struct prime_node *node = generate_prim_array(PRIME_COUNT);
	print_prim_list(node);

	return 0;
}

static void __exit prime_list_exit(void)
{
	struct prime_node *node;
	struct prime_node *temp;
	printk(KERN_INFO "Prime-list module unloaded\n");

	list_for_each_entry_safe(node, temp, &prime_list, list) {
		printk(KERN_INFO "freeing Prime number: %d\n", node->prime);
		list_del(&node->list);
		kfree(node);
	}
}

module_init(prime_list_init);
module_exit(prime_list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Prime-list module description");
MODULE_VERSION("Prime-list module version");
MODULE_AUTHOR("wuyh");