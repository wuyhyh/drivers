/******************************************************************************
 *
 * Name: call_usermode_helper.c - Description
 * Created on 2025/03/17
 * Copyright (C) 2022 - 2025, wyh.
 *
*****************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/unistd.h>

// 线程指针
static struct task_struct *thread;

// 内核线程函数
static int call_userprog_thread(void *data)
{
	static char *argv[] = {
		"/home/wuyuhang/CLionProjects/drivers/call_usermode_helper/hello.sh",
		"argv1", "argv2", NULL };
	static char *envp[] = { "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin",NULL };
	pr_info("kernel module: calling usermode helper ...\n");
	call_usermodehelper(argv[0], argv, envp,UMH_WAIT_EXEC);

	return 0;
}

static int __init call_userprog_thread_init(void)
{
	pr_info("kernel module loaded.\n");

	// 创建并启动内核线程
	thread = kthread_run(call_userprog_thread, NULL, "userprog_thread");
	if (IS_ERR(thread)) {
		pr_err("failed to create kernel thread.\n");
		return PTR_ERR(thread);
	}

	return 0;
}

static void __exit call_userprog_thread_exit(void)
{
	pr_info("kernel module unloaded.\n");
}

module_init(call_userprog_thread_init);
module_exit(call_userprog_thread_exit);
MODULE_LICENSE("GPL");