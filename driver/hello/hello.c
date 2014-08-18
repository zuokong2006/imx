
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	printk(KERN_INFO "Module hello world init.\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Module hello world exit.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("zuokong");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A sample Hello world Module");
