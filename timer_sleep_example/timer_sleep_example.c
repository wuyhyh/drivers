/******************************************************************************
 *
 * Name: timer_sleep_example.c - Description
 * Created on 2025/03/17
 * Copyright (C) 2022 - 2025, wyh.
 *
 *****************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>

#define TIMER_INTERVAL_SEC 2 // 定时器周期2秒

static struct timer_list my_timer;
static struct work_struct my_work;

// 工作队列处理函数
static void work_handler(struct work_struct *work)
{
	if (work != &my_work) {
		pr_err("illegal work struct, it possibly has been released.\n");
		return;
	}

	pr_info("work queue is executing, sleep for 2 seconds.\n");
	msleep(2000);
	pr_info("work queue is executing, sleep finish.\n");
}

// 定时器回调函数
static void timer_callback(struct timer_list *timer)
{
	// 确保callback只在模块有效时运行
	if (!try_module_get(THIS_MODULE)) {
		pr_err("module not available\n");
		return;
	}

	pr_info("timer trigger, schedule work queue.\n");
	schedule_work(&my_work);
	// 重新设置定时器
	mod_timer(&my_timer, jiffies + TIMER_INTERVAL_SEC * HZ);

	module_put(THIS_MODULE);
}

static int __init timer_sleep_example_init(void)
{
	pr_info("timer_sleep_example_init\n");

	INIT_WORK(&my_work, work_handler); // 初始化工作队列
	timer_setup(&my_timer, timer_callback, 0); // 初始化定时器
	mod_timer(&my_timer, jiffies + TIMER_INTERVAL_SEC * HZ); // 设置定时器首次触发时间

	return 0;
}

static void __exit timer_sleep_example_exit(void)
{
	del_timer_sync(&my_timer); // 删除定时器，确保不会再次触发
	cancel_work_sync(&my_work); // 确保工作队列被取消
	pr_info("timer_sleep_example_exit\n");
}

module_init(timer_sleep_example_init);
module_exit(timer_sleep_example_exit);

MODULE_LICENSE("GPL");