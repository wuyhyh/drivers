/******************************************************************************
 *
 * Name: tasklet_example.c - Description
 * Created on 2025/03/17
 * Copyright (C) 2022 - 2025, wyh.
 *
 *****************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static void my_tasklet_handler(unsigned long data)
{
	printk(KERN_INFO "tasklet func executed, data = %ld.\n", data);
}

struct tasklet_struct my_tasklet;

static int __init tasklet_example_init(void)
{
	printk(KERN_INFO "tasklet example init.\n");

	// 初始化tasklet
	tasklet_init(&my_tasklet, my_tasklet_handler, 1234);

	// 调度tasklet，任务会异步执行
	tasklet_schedule(&my_tasklet);

	return 0;
}

static void __exit tasklet_example_exit(void)
{
	tasklet_kill(&my_tasklet); // 释放tasklet资源
	printk(KERN_INFO "tasklet example exit.\n");
}

module_init(tasklet_example_init);
module_exit(tasklet_example_exit);
MODULE_LICENSE("GPL");