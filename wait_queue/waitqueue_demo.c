/******************************************************************************
 *
 * Name: waitqueue_demo.c - Description
 * Created on 2025/03/16
 * Copyright (C) 2022 - 2025, wyh.
 *
 *****************************************************************************/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define DEVICE_NAME "waitqueue_demo"

static int major;
static struct cdev my_cdev;
static struct class *my_class;
static wait_queue_head_t my_waitqueue;
static int flag = 0; // 用于标记事件是否发生

// 设备文件 open
static int my_open(struct inode *inode, struct file *file);

// 设备文件 read
static ssize_t my_read(struct file *file, char __user *buf, size_t count,
			loff_t *ppos);

// 设备文件 write
static ssize_t my_write(struct file *file, const char __user *buf, size_t count,
			loff_t *ppos);

// 设备文件 release
static int my_release(struct inode *inode, struct file *file);

// 设备文件 operations
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

// 模块初始化
static int __init my_init(void)
{
	dev_t dev;
	int ret;

	// 申请设备号
	ret = alloc_chrdev_region(&dev, 0, 1,DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ALERT "alloc_chrdev_region failed\n");
		return ret;
	}

	major = MAJOR(dev);
	pr_info("Registered device with major number = %d\n", major);

	// 初始化字符设备
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;

	ret = cdev_add(&my_cdev, dev, 1);
	if (ret < 0) {
		printk(KERN_ALERT "cdev_add failed\n");
		unregister_chrdev_region(dev, 1);
		return ret;
	}

	// 创建设备
	my_class = class_create(DEVICE_NAME);
	if (IS_ERR(my_class)) {
		printk(KERN_ALERT "class_create failed\n");
		cdev_del(&my_cdev);
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(my_class);
	}

	// 创建设备节点
	if (!device_create(my_class, NULL, dev, NULL,DEVICE_NAME)) {
		pr_err("device create failed\n");
		class_destroy(my_class);
		cdev_del(&my_cdev);
		unregister_chrdev_region(dev, 1);
		return -ENODEV;
	}

	// 初始化等待队列
	init_waitqueue_head(&my_waitqueue);

	printk(KERN_INFO "Wait queue successfully initialized\n");
	return 0;
}

// 模块卸载
static void __exit my_exit(void)
{
	dev_t dev = MKDEV(MAJOR(major), 0);

	// 删除设备节点
	device_destroy(my_class, dev);
	class_destroy(my_class);

	// 删除字符设备
	cdev_del(&my_cdev);

	// 释放设备号
	unregister_chrdev_region(dev, 1);

	printk(KERN_INFO "Wait queue unloading\n");
}

// 设备文件 open
static int my_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Opening device\n");
	return 0;
}

// 设备文件 read
static ssize_t my_read(struct file *file, char __user *buf, size_t count,
			loff_t *ppos)
{
	pr_info("waitqueue_demo: process(%d) is going to sleep\n",
		current->pid);

	//让当前进程进入等待队列，直到flag变为非0
	wait_event_interruptible(my_waitqueue, flag != 0);

	pr_info("waitqueue_demo: Process %d is woken up\n", current->pid);

	flag = 0; // 以便下次等待

	return 0;
}

// 设备文件 write
static ssize_t my_write(struct file *file, const char __user *buf, size_t count,
			loff_t *ppos)
{
	pr_info("waitqueue_demo: waking up sleeping processes\n");

	// 设置flag并唤醒所有等待进程
	flag = 1;
	wake_up_interruptible(&my_waitqueue);

	pr_info("waitqueue_demo: Process %d is woken up\n", current->pid);
	return count;
}

// 设备文件 release
static int my_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "waitqueue_demo: releasing device\n");
	return 0;
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Wait queue demo driver");
MODULE_VERSION("Wait queue demo version");