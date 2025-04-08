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


#define CDEV_NAME       "dev_key_gpio_21"
#define CDEV_CNT        1
// #define DTS_NODE        "/key_gpio_21"
#define DTS_COMPATIBLE  "led_gpio_21"
#define PRESSED         1
#define NOPRESSED       0

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

    int key_gpio;
    atomic_t key_val;

};

struct cdev_dev key_cdev;

static int cdev_open(struct inode *inode, struct file *filp)
{
    filp->private_data =  &key_cdev;
    return 0;
}
static int cdev_release(struct inode *inode, struct file *filp)
{

    return 0;
}
static ssize_t cdev_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret;
    int value;
    struct cdev_dev *dev = filp->private_data;
    if(gpio_get_value(key_cdev.key_gpio) == 0){
        while (!gpio_get_value(dev->key_gpio));
        atomic_set(&dev->key_val, PRESSED);
    }
    else{
        atomic_set(&dev->key_val, NOPRESSED);
    }
    value = atomic_read(&dev->key_val);
    ret = copy_to_user(buf, &value, sizeof(value));
    return 0;
}

static ssize_t cdev_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    
    return 0;
}

static struct file_operations cdev_fops = {
    .owner = THIS_MODULE,
    .open = cdev_open,
    .release = cdev_release,
    .write = cdev_write,
    .read = cdev_read,
};

//key gpio
static int key_gpio_init(struct platform_device *pdev){
    int ret;
    key_cdev.nd = pdev->dev.of_node;
    // key_cdev.nd = of_find_node_by_path(DTS_NODE);
    key_cdev.key_gpio = of_get_named_gpio(key_cdev.nd, "gpios", 0);
    printk("key_gpio = %d\n", key_cdev.key_gpio);
    gpio_request(key_cdev.key_gpio, "key_gpio_request");
    ret = gpio_direction_input(key_cdev.key_gpio); // 默认输出高电平
    return 0;
}
//key cdev
static int key_dev_init(void){
    atomic_set(&key_cdev.key_val, NOPRESSED);
    //1. 注册设备号
    if(key_cdev.major){
        key_cdev.cdev_nr = MKDEV(key_cdev.major, 0);
        register_chrdev_region(key_cdev.cdev_nr, CDEV_CNT, CDEV_NAME);
    }else{
        alloc_chrdev_region(&key_cdev.cdev_nr, 0, CDEV_CNT, CDEV_NAME);
        key_cdev.major = MAJOR(key_cdev.cdev_nr);
        key_cdev.minor = MINOR(key_cdev.cdev_nr);
    }
    printk("major = %d, minor = %d\r\n", key_cdev.major, key_cdev.minor);

    //2. 初始化cdev
    cdev_init(&key_cdev.cdev, &cdev_fops);
    cdev_add(&key_cdev.cdev, key_cdev.cdev_nr, CDEV_CNT);

    key_cdev.class = class_create(CDEV_NAME);
    key_cdev.device = device_create(key_cdev.class, NULL, key_cdev.cdev_nr, NULL, CDEV_NAME);
    return 0;
}
static int key_dev_exit(void){
    gpio_free(key_cdev.key_gpio);

    cdev_del(&key_cdev.cdev);
    unregister_chrdev_region(key_cdev.cdev_nr, CDEV_CNT);
    device_destroy(key_cdev.class, key_cdev.cdev_nr);
    class_destroy(key_cdev.class);
    return 0;

}
/*=====================start of platform driver=====================*/
//初始化在probe函数中实现
static int key_probe(struct platform_device *pdev){
    printk("platform is probed\n");
    key_dev_init();
    key_gpio_init(pdev);

    return 0;
}
//note : `void` type of the `remove` function
static void key_remove(struct platform_device *pdev){
    printk("platform is removed \n");
    key_dev_exit();
}

static const struct of_device_id of_match_key[] = {
    {.compatible = DTS_COMPATIBLE},
    {}

};

static struct platform_driver key_pdriver = {
    .driver = {
        .name = "key-pdriver",
        .of_match_table = of_match_key,
    },
    .probe = key_probe,
    .remove = key_remove,
};

static int __init key0_init(void){
    printk("hello cdev\n");
    platform_driver_register(&key_pdriver);
    return 0;

}

static void __exit key0_exit(void){
    printk("goodbye cdev\n");
    platform_driver_unregister(&key_pdriver);

}
/*=====================end of platform driver=====================*/

module_init(key0_init);
module_exit(key0_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XWJ");