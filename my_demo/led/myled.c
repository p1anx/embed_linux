#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define MYLED_CNT 1
#define MYLED_NAME "myled"
#define LEDON   0
#define LEDOFF  1





struct myled_dev{
    dev_t devid;
    int major;
    int minor;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int led_gpio;



};

struct myled_dev myled;


static int led_open(struct inode *inode, struct file *filp){
    filp->private_data = &myled;
    return 0;
}

static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt){
    return 0;
}
static int led_release(struct inode *inode, struct file *filp){
    return 0;
}

static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt){
    int retvalue;
    unsigned char databuf[1];
    unsigned char ledstat;
    struct myled_dev *dev = filp->private_data;

    retvalue = copy_from_user(databuf, buf, cnt);
    if(retvalue < 0){
        printk("kernel write failed\r\n");
        return -EFAULT;
    }

    ledstat = databuf[0];
    if(ledstat == LEDON){
        gpio_set_value(dev->led_gpio, 0);
    }
    else if(ledstat == LEDOFF){
        gpio_set_value(dev->led_gpio, 1);
    }
    return 0;
}

static const struct file_operations myled_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .write =  led_write,
    .read = led_read,
    .release = led_release,
};
static int __init myled_init(void){
    int ret = 0;

    printk("The led moduel is load\n");
    myled.nd = of_find_node_by_path("/myled");
    if(myled.nd == NULL){
        printk("gpio node can not found\r\n");
        return -EINVAL;
    }
    else{
        printk("gpio led nod has been found\r\n");
    }

    myled.led_gpio =  of_get_named_gpio(myled.nd, "led-gpio", 0);
    if(myled.led_gpio < 0){
        printk("can not get led-gpio");
        return -EINVAL;
    }
    printk("led-gpio num = %d\r\n", myled.led_gpio);

    ret = gpio_direction_output(myled.led_gpio, 1);
    if(ret < 0){
        printk("cant set gpio\r\n");
    }



    myled.major = 0 ;
    if(myled.major){
        myled.devid = MKDEV(myled.major, 0);  
        register_chrdev_region(myled.devid, MYLED_CNT, MYLED_NAME);

    }
    else{
        alloc_chrdev_region(&myled.devid, 0, MYLED_CNT, MYLED_NAME);
        myled.major = MAJOR(myled.devid);
        myled.minor = MINOR(myled.devid);
    }
    myled.cdev.owner = THIS_MODULE;
    cdev_init(&myled.cdev, &myled_fops);
    cdev_add(&myled.cdev, myled.devid, MYLED_CNT);
    myled.class = class_create(THIS_MODULE,MYLED_NAME);
    myled.device = device_create(myled.class, NULL, myled.devid, NULL, MYLED_NAME);



    return ret;
}

static void __exit myled_exit(void){
    printk("The moduel is remove\n");
    cdev_del(&myled.cdev);
    unregister_chrdev_region(myled.devid, MYLED_CNT);
    device_destroy(myled.class, myled.devid);
    class_destroy(myled.class);

}


module_init(myled_init);
module_exit(myled_exit);
MODULE_LICENSE("GPL");
