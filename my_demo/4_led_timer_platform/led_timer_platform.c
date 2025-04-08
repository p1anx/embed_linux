#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/of_gpio.h>
#include <linux/uacce.h>

#include <linux/platform_device.h>

/*==============start of timer==============*/
#include <linux/timer.h>
#include <asm/io.h>
/*==============end of timer==============*/

#define CDEV_NAME       "timer_gpio_14"
#define CDEV_CNT        1
#define DTS_NODE        "/led_gpio_14"
#define LEDON           1
#define LEDOFF          0
#define DTS_COMPATIBLE  "led_3"

/*==============start of timer==============*/
#define CLOSE_CMD       (_IO(0XEF, 0X1))
#define OPEN_CMD        (_IO(0XEF, 0X2))
#define PERIOD_CMD      (_IO(0XEF, 0X3))



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
    
/*==============start of timer==============*/
    int timer_period;
    struct timer_list timer;
    spinlock_t lock;
/*==============end of timer==============*/

};

struct cdev_dev led_cdev;

/*==============start of timer==============*/
static long timer_unlocked_ioctl(struct file *filp,  unsigned int cmd, unsigned long arg){
    struct cdev_dev *dev = (struct cdev_dev *)filp->private_data;
    // struct cdev_dev *dev = container_of(filp->private_data, struct cdev_dev, cdev);
    unsigned long flags;
    int timer_period;

    switch (cmd)
    {
    case CLOSE_CMD:
        del_timer_sync(&dev->timer);
        break;
    case OPEN_CMD:
        spin_lock_irqsave(&dev->lock, flags);
        timer_period = dev->timer_period;
        spin_unlock_irqrestore(&dev->lock, flags);
        mod_timer(&dev->timer, jiffies + msecs_to_jiffies(timer_period));
        break;
    case PERIOD_CMD:
        spin_lock_irqsave(&dev->lock, flags);
        dev->timer_period = arg;
        spin_unlock_irqrestore(&dev->lock, flags);
        mod_timer(&dev->timer, jiffies + msecs_to_jiffies(arg));
        break;
    default:
        break;
    }
    return 0;

}

void timer_function(struct timer_list *tim){
    struct cdev_dev *dev = from_timer(dev, tim, timer);//使用 from_timer 宏从 timer 参数中获取 dev 结构体的指针
    int timer_period;
    static int status = 1;
    unsigned long flags;

    printk("timer is starting\n");
    status = !status;
    gpio_set_value(dev->led_gpio, status);

    /*重启定时器*/
    spin_lock_irqsave(&dev->lock, flags);
    timer_period = dev->timer_period;
    spin_unlock_irqrestore(&dev->lock, flags);
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(dev->timer_period));

}

static int timer0_init(void){
    spin_lock_init(&led_cdev.lock);


    led_cdev.timer_period = 1000;
    timer_setup(&led_cdev.timer, timer_function, 0);
    led_cdev.timer.expires= jiffies + msecs_to_jiffies(led_cdev.timer_period);
    add_timer(&led_cdev.timer);
    return 0;
}
/*==============end of timer==============*/

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
    .unlocked_ioctl = timer_unlocked_ioctl,
};

//led gpio
static int led_gpio_init(struct platform_device *pdev){
    int ret;
    led_cdev.nd = pdev->dev.of_node;
    // led_cdev.nd = of_find_node_by_path(DTS_NODE);
    led_cdev.led_gpio = of_get_named_gpio(led_cdev.nd, "gpios", 0);
    gpio_request(led_cdev.led_gpio, "led_gpio21");
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
static int led_dev_exit(void){
/*==============start of timer==============*/
    /*删除timer*/
    del_timer_sync(&led_cdev.timer);

    gpio_free(led_cdev.led_gpio);

    cdev_del(&led_cdev.cdev);
    unregister_chrdev_region(led_cdev.cdev_nr, CDEV_CNT);
    device_destroy(led_cdev.class, led_cdev.cdev_nr);
    class_destroy(led_cdev.class);
    return 0;

}


/*=====================start of platform driver=====================*/
//初始化在probe函数中实现
static int led_probe(struct platform_device *pdev){
    printk("timer platform is probed\n");
    led_dev_init();

    led_gpio_init(pdev);

/*==============start of timer==============*/
    timer0_init();

    return 0;
}
//note : `void` type of the `remove` function
static void led_remove(struct platform_device *pdev){
    printk("timer platform is removed \n");
    led_dev_exit();
}

static const struct of_device_id of_match_led[] = {
    {.compatible = DTS_COMPATIBLE},
    {}

};

static struct platform_driver led_pdriver = {
    .driver = {
        .name = "led-pdriver",
        .of_match_table = of_match_led,
    },
    .probe = led_probe,
    .remove = led_remove,
};

static int __init led_init(void){
    printk("hello timer\n");
    platform_driver_register(&led_pdriver);
    return 0;

}

static void __exit led_exit(void){
    printk("goodbye cdev\n");
    platform_driver_unregister(&led_pdriver);

}
/*=====================end of platform driver=====================*/

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XWJ");