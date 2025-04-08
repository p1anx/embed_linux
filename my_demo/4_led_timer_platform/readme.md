## init_timer()被移除,如果linux内核版本小于4.15
### timer_setup()
```d
#if 如果Linux内核版本 < KERNEL_VERSION(4,14,0)
    init_timer(&dev->getIntrTimer);
    dev->getIntrTimer.data = (unsigned long) dev;
    dev->getIntrTimer.function = GetIntrTimerCallback;
    /* ... */
    add_timer(&dev->getIntrTimer);
#else
    timer_setup(&dev->getIntrTimer, GetIntrTimerCallback, 0);
    /* the third argument may include TIMER_* flags */
    /* ... */
#endif
```
### callback

```d
#if 如果Linux内核版本 < KERNEL_VERSION(4,14,0)
void GetIntrTimerCallback(unsigned long devAddr)
{
    myDevice *dev = (myDevice *) devAddr;
#else
void GetIntrTimerCallback(struct timer_list *t)
{
    myDevice *dev = from_timer(dev, t, getIntrTimer);
#endif
    /* Do something with "dev" */

```

## 代码注释
### 
```c
struct cdev_dev *dev = from_timer(dev, timer, timer);//使用 from_timer 宏从 timer 参数中获取 dev 结构体的指针
```
struct cdev_dev *dev = from_timer(dev, timer, timer);: 这行代码使用 from_timer 宏从 struct timer_list *timer 参数中获取包含定时器的自定义结构体 struct cdev_dev 的指针。
struct cdev_dev *dev: 定义了一个 struct cdev_dev 类型的指针 dev，用于存储获取到的结构体地址。 struct cdev_dev 结构体很可能包含定时器 timer，以及 LED 控制所需的 GPIO 引脚信息和其他状态。
from_timer(dev, timer, timer): from_timer 是一个宏，用于从 struct timer_list 指针获取包含该定时器的结构体的指针。
dev: 是目标结构体的变量名，宏会根据这个名字自动推断变量类型.
timer: 指向 struct timer_list 结构体的指针。
timer: 是 struct cdev_dev 结构体中 struct timer_list 成员的名称。
重要: from_timer 宏会计算出 struct cdev_dev 结构体的起始地址，并将其赋值给 dev 指针。 这允许你在回调函数中访问 struct cdev_dev 结构体的所有成员。

###
```c
    /*重启定时器*/
    spin_lock_irqsave(&dev->lock, flags);
    timer_period = dev->timer_period;
    spin_unlock_irqrestore(&dev->lock, flags);
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(dev->timer_period));
```
spin_lock_irqsave(&dev->lock, flags);: 获取自旋锁，并禁用中断。 这用于保护 dev->timer_period 成员，防止并发访问。
&dev->lock: 指向 struct cdev_dev 结构体中的一个自旋锁。
flags: 用于保存中断标志，以便在 spin_unlock_irqrestore 中恢复。
重要: 自旋锁用于保护共享资源，防止多个线程或中断同时访问。 spin_lock_irqsave 会禁用中断，以防止中断处理程序在持有自旋锁时抢占当前线程。
timer_period = dev->timer_period;: 从 dev 结构体中读取 timer_period 成员的值，并将其赋值给局部变量 timer_period。 这个 timer_period 成员可能表示定时器的周期 (以毫秒为单位)。 在获取自旋锁的保护下读取这个值，可以防止在读取期间被其他线程或中断修改。
spin_unlock_irqrestore(&dev->lock, flags);: 释放自旋锁，并恢复之前的中断状态。
mod_timer(&dev->timer, jiffies + msecs_to_jiffies(dev->timer_period));: 使用 mod_timer 函数修改定时器的超时时间，并重新启动定时器。
&dev->timer: 指向要修改的定时器结构体。
jiffies + msecs_to_jiffies(dev->timer_period): 计算新的超时时间。
jiffies: 内核启动以来经过的时钟节拍数。
msecs_to_jiffies(dev->timer_period): 将毫秒转换为时钟节拍数。 dev->timer_period 存储了以毫秒为单位的定时器周期。
重要: mod_timer 是一个原子操作，可以安全地修改正在运行的定时器的超时时间。 使用 mod_timer 可以避免出现竞态条件。 应该总是优先使用 mod_timer 而不是先 del_timer 然后 add_timer。

## ioctl()
```c

#include <asm/io.h>

#define CLOSE_CMD       (_IO(0XEF, 0X1))
#define OPEN_CMD        (_IO(0XEF, 0X2))
#define PERIOD_CMD      (_IO(0XEF, 0X3))

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
static struct file_operations cdev_fops = {
    .owner = THIS_MODULE,
    .open = cdev_open,
    .release = cdev_release,
    .write = cdev_write,
    .read = cdev_read,
    .unlocked_ioctl = timer_unlocked_ioctl, //add this
};
```