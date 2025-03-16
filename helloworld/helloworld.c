#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init helloworld_init(void)
{
	printk(KERN_INFO "Hello world\n");
	return 0;
}

static void __exit helloworld_exit(void)
{
	printk(KERN_INFO "Goodbye world\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("v1.0");
MODULE_DESCRIPTION("Hello world");
MODULE_AUTHOR("wuyh");