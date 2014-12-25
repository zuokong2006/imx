/**
  file：globalmem.c
  author：zuokongxiao
  字符设备启动测试模块
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

//大小定义4K
#define GLOBALMEM_SIZE          4096
//主设备号定义
#define GLOBALMEN_MAJOR         254
//设备名定义
#define GLOBALMEN_NAME          "globalmem"

static globalmem_major = GLOBALMEN_MAJOR;
//设备结构体定义
struct globalmem_dev
{
    struct cdev cdev; /* cdev结构体 */
    unsigned char mem[GLOBALMEM_SIZE];
};
//设备结构体指针定义
struct globalmem_dev *globalmen_devp;








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
static const struct file_operations globalmen_fops = 
{
    .owner = THIS_MODULE,
    .llseek = ;
    .read = ;
    .write = ;
    .ioctl = ;
    .open = ;
    .release = ;
};

/* 初始化并注册cdev */
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err = 0, devno = MKDEV(globalmem_major, index);

    cdev_init(&(dev->cdev), &globalmen_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &globalmen_fops;
    err = cdev_add(&(dev->cdev), devno, 1);
    if(err)
    {
        printk(KERN_NOTICE"cdev_add err = %d, devno = %d.\r\n", err, devno);
    }
}

/* 模块加载函数 */
static int __init globalmem_init(void)
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
static void __exit globalmen_exit(void)
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

module_init(globalmen_init);
module_exit(globalmen_exit);

MODULE_AUTHOR("zuokong");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Globalmem is char driver module.");



