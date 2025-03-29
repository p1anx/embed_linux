#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <asm/io.h>

static struct class *pin4_class;
static struct device *pin4_class_dev;
static dev_t devno;
static int major = 231;
static int minor = 0;
static char *module_name = "pin4";

volatile unsigned int *GPFSEL0 = NULL;
volatile unsigned int *GPSET0 = NULL;
volatile unsigned int *GPCLR0 = NULL;

static ssize_t pin4_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
  printk("pin4_read\n");
  return 0;
}

static int pin4_open(struct inode *inode, struct file *file)
{
  printk("pin4_open\n");
  *GPFSEL0 &= ~(0x6 << 12);
  *GPFSEL0 |=  (0x1 << 12);
  return 0;
}

static ssize_t pin4_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
  char userCmd;
  if(copy_from_user(&userCmd, buf, count) == 0)
  printk("get value\n");
  if(userCmd == 1)
  {
    printk("set 1\n");
    *GPSET0 |= 0x1 << 4;
  }
  else if(userCmd == 0){
    printk("set 0\n");
    *GPCLR0 |= 0x1 << 4;
  }
  else {
    printk("undo\n");
  }
  return 0;
}

static struct file_operations pin4_fops = {
  .owner = THIS_MODULE,
  .open = pin4_open,
  .write = pin4_write,
  .read = pin4_read,
};

int __init pin4_drv_init(void)
{
  int ret;
  devno = MKDEV(major, minor);
  ret = register_chrdev(major, module_name, &pin4_fops);

 // pin4_class = class_create(THIS_MODULE, "myfirstdemo");
  pin4_class = class_create("myfirstdemo");
  pin4_class_dev = device_create(pin4_class, NULL, devno, NULL, module_name);
  GPFSEL0 = (volatile unsigned int*) ioremap(0xfe2000000, 4);
  GPSET0 = (volatile unsigned int*) ioremap(0xfe200001c, 4);
  GPCLR0 = (volatile unsigned int*) ioremap(0xfe2000028, 4);
  return 0;
}

void __exit pin4_drv_exit(void)
{
  iounmap(GPFSEL0);
  iounmap(GPSET0);
  iounmap(GPCLR0);
  device_destroy(pin4_class, devno);
  class_destroy(pin4_class);
  unregister_chrdev(major, module_name);
}

module_init(pin4_drv_init);
module_exit(pin4_drv_exit);
MODULE_LICENSE("GPL v2");




