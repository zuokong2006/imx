
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

#define TIMEOUT			2 /* 2秒 */
#define TIMER_EXPIRES	(jiffies+TIMEOUT*HZ)

static struct timer_list s_stTimer = {0};

/* 定时到所执行的函数 */
void timer_cb(unsigned long ulArg)
{
	printk("Time up!\n");
	mod_timer(&s_stTimer, TIMER_EXPIRES);
}

/* 模块加载函数 */
static int __init timer_init(void)
{
	init_timer(&s_stTimer);
	s_stTimer.expires = TIMER_EXPIRES;
	s_stTimer.function = timer_cb;
	s_stTimer.data = 0;
	add_timer(&s_stTimer);
	printk("Timer init!\n");
	
	return 0;
}

/* 模块卸载函数 */
static void __exit timer_exit(void)
{
	del_timer(&s_stTimer);
	printk("Timer exit!\n");
}

module_init(timer_init);
module_exit(timer_exit);

MODULE_AUTHOR("zuokong");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Timer test module");



