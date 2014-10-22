
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

#define TIMEOUT			2 /* 2秒 */
#define TIMER_EXPIRES	(jiffies+TIMEOUT*HZ)
/* 设备名 */
#define DEVICE_NAME		"bb_led"

/* led */
static const unsigned long bb_led_tab[] = {53, 54, 55, 56};
struct timer_list s_stTimer;
int lastio = 0;
dev_t led_devno;
struct cdev led_cdev;


/* 定时到所执行的函数 */
void timer_cb(unsigned long ulArg)
{
	int io = 0;

	//printk("Time up!\n");
	mod_timer(&s_stTimer, TIMER_EXPIRES);
	io = gpio_get_value(bb_led_tab[3]);
	//printk("io=%d\r\n", io);
	if(0 != lastio)
	{
		lastio = 0;
	}
	else
	{
		lastio = 1;
	}
	gpio_set_value(bb_led_tab[3], lastio);
}

/* open */
int bb_led_open(struct inode *inode, struct file *filp)
{
	printk("led device is open!\r\n");
	return 0;
}

/* release */
int bb_led_release(struct inode *inode, struct file *filp)
{
	printk("led device is release!\r\n");
	return 0;
}

/* ioctl */
long bb_led_ioctl(struct file *file, unsigned int cmd, unsigned long arg) 
{
	long ret = 0;
	
	printk("ioctl fn: cmd=%d\r\n", cmd);
	switch(cmd)
	{
		case 0:
			gpio_set_value(bb_led_tab[1], 0);
			break;
		case 1:
			gpio_set_value(bb_led_tab[1], 1);
			break;
		default:
			ret = -1;
			break;
	}
	
	return ret;
}

/* 文件操作结构体 */
static struct file_operations led_dev_fops = 
{
	.owner = THIS_MODULE,
	.unlocked_ioctl = bb_led_ioctl,
	.open = bb_led_open,
	.release = bb_led_release,
};

/* 模块加载函数 */
static int __init bb_led_init(void)
{
	int ret = 0, err = 0;
	//int dev_no = 0;

	/* 动态分配设备号 */
	ret = alloc_chrdev_region(&led_devno, 0, 1, "bb_led");
	if(0 > ret)
	{
		printk("alloc_chrdev_region return < 0\r\n");
		return -1;
	}
	//dev_no = MAJOR(led_devno);
	
	cdev_init(&led_cdev, &led_dev_fops);
	led_cdev.owner = THIS_MODULE;
	led_cdev.ops = &led_dev_fops;
	//err = cdev_add(&led_cdev, MKDEV(dev_no, 0), 1);
	err = cdev_add(&led_cdev, led_devno, 1);
	if(err)
	{
		printk("err=%d adding led_cdev!\r\n", err);
	}
	else
	{
		printk("success adding led_cdev!\r\n");
	}
	
	gpio_set_value(bb_led_tab[3], 1);
	gpio_set_value(bb_led_tab[1], 1);
	/* 初始化定时器 */
	init_timer(&s_stTimer);
	s_stTimer.expires = TIMER_EXPIRES;
	s_stTimer.function = timer_cb;
	s_stTimer.data = 0;
	add_timer(&s_stTimer);
	printk("led driver init!\n");
	
	return 0;
}

/* 模块卸载函数 */
static void __exit bb_led_exit(void)
{
	//int dev_no = MAJOR(led_devno);;
	
	gpio_set_value(bb_led_tab[3], 0);
	gpio_set_value(bb_led_tab[1], 0);
	del_timer(&s_stTimer);
	
	cdev_del(&led_cdev);
	//unregister_chrdev_region(MKDEV(dev_no, 0), 1);
	unregister_chrdev_region(led_devno, 1);
	
	printk("led driver exit!\n");
}

module_init(bb_led_init);
module_exit(bb_led_exit);

MODULE_AUTHOR("zuokong");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Timer test module");



