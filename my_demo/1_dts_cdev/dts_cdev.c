#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/of_gpio.h>
#include <linux/uacce.h>


#define CDEV_NAME       "led3_cdev"
#define CDEV_CNT        1
#define DTS_NODE        "/led_3"
#define LEDON           1
#define LEDOFF          0

struct cdev_dev{
/*==============basic==============*/
    dev_t cdev_nr;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int major;
    int minor;
/*================================*/

    int led_gpio;

};

struct cdev_dev led_cdev;

static int cdev_open(struct inode *inode, struct file *filp)
{
    filp->private_data =  &led_cdev;
    return 0;
}
static int cdev_release(struct inode *inode, struct file *filp)
{

    return 0;
}
static ssize_t cdev_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    return 0;
}

static ssize_t cdev_write(struct file *flip, const char __user *buf, size_t cnt, loff_t *offt)
{
    int ret;
    struct cdev_dev *dev = flip->private_data;
    unsigned char write_buf[1];

    ret = copy_from_user(write_buf, buf, cnt);
    if(ret < 0){
        printk("failed to write\n");
        return -EFAULT;
    }
    if(write_buf[0] == LEDON){
        gpio_set_value(dev->led_gpio, 1);
    }
    else if(write_buf[0] == LEDOFF){
        gpio_set_value(dev->led_gpio, 0);
    }
    else
    {
        printk("invalid value, set 0 or 1\n");
    }
    
    return 0;
}

static struct file_operations cdev_fops = {
    .owner = THIS_MODULE,
    .open = cdev_open,
    .release = cdev_release,
    .write = cdev_write,
    .read = cdev_read,
};

//led gpio
static int led_gpio_init(void){
    int ret;
    led_cdev.nd = of_find_node_by_path(DTS_NODE);
    led_cdev.led_gpio = of_get_named_gpio(led_cdev.nd, "gpios", 0);
    ret = gpio_direction_output(led_cdev.led_gpio, 1); // 默认输出高电平
    return 0;
}
//led cdev
static int led_dev_init(void){
    //1. 注册设备号
    if(led_cdev.major){
        led_cdev.cdev_nr = MKDEV(led_cdev.major, 0);
        register_chrdev_region(led_cdev.cdev_nr, CDEV_CNT, CDEV_NAME);
    }else{
        alloc_chrdev_region(&led_cdev.cdev_nr, 0, CDEV_CNT, CDEV_NAME);
        led_cdev.major = MAJOR(led_cdev.cdev_nr);
        led_cdev.minor = MINOR(led_cdev.cdev_nr);
    }
    printk("major = %d, minor = %d\r\n", led_cdev.major, led_cdev.minor);

    //2. 初始化cdev
    cdev_init(&led_cdev.cdev, &cdev_fops);
    cdev_add(&led_cdev.cdev, led_cdev.cdev_nr, CDEV_CNT);

    led_cdev.class = class_create(CDEV_NAME);
    led_cdev.device = device_create(led_cdev.class, NULL, led_cdev.cdev_nr, NULL, CDEV_NAME);
    return 0;
}

static int __init led_cdev_init(void){
    printk("hello cdev\n");
    
    led_dev_init();
    led_gpio_init();
    return 0;

}

static void __exit led_cdev_exit(void){
    printk("goodbye cdev\n");
    cdev_del(&led_cdev.cdev);
    unregister_chrdev_region(led_cdev.cdev_nr, CDEV_CNT);
    device_destroy(led_cdev.class, led_cdev.cdev_nr);
    class_destroy(led_cdev.class);
}

module_init(led_cdev_init);
module_exit(led_cdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XWJ");