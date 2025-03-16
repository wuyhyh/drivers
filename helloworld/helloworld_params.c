//
// Created by wuyuhang on 25-3-16.
//

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static char *mystr = "hello";
static int myint = 1;
static int myarray[4] = { 0, 1, 2, 3 };

module_param(myint, int, S_IRUGO);
module_param(mystr, charp, S_IRUGO);
module_param_array(myarray, int, NULL, S_IWUSR | S_IRUSR);

MODULE_PARM_DESC(myint, "this is my int variable");
MODULE_PARM_DESC(myarray, "this is my array variable");
MODULE_PARM_DESC(mystr, "this is my string variable");

static int foo(void)
{
	pr_info("mystring is a string: %s\n", mystr);
	pr_info("array elements: %d\t%d\t%d\t%d\n", myarray[0], myarray[1],
		myarray[2], myarray[3]);
	return myint;
}

static int __init hello_init(void)
{
	int ret = foo();
	pr_info("hello world, ret = %d\n", ret);
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("end of the world\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello world");
MODULE_VERSION("v1.0");
MODULE_AUTHOR("wuyh");