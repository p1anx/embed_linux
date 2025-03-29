#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAN!");
MODULE_DESCRIPTION("What Can I Say?");

#define BCM2712_PCIE_BASE   0x1f00000000UL
#define IO_BANK0_BASE       (BCM2712_PCIE_BASE + 0xd0000)
#define PADS_BANK0_BASE     (BCM2712_PCIE_BASE + 0xf0000)
#define SYS_RIO0_BASE       (BCM2712_PCIE_BASE + 0xe0000)

#define GPIO14_CTRL         (IO_BANK0_BASE   + 0x074) // GPIO14 control register
#define PADS14_CTRL         (PADS_BANK0_BASE + 0x03c) // PAD14  control register
#define BIT_PADS_OD         (0x1  <<  7) // Output Disable
#define BIT_PADS_IE         (0x1  <<  6) // Input  Enable
#define BIT_IO_OEOVER       (0x3  << 14) // output enable
#define BIT_IO_OUTOVER      (0x3  << 12) // drive output high

// init exit
static void __iomem *gpio_ctrl = 0;
static void __iomem *pad_ctrl  = 0;
static int bak;

static int __init rgb_led_init(void)
{
    int pdctrl, ioctrl;
    // get the vm addr
    gpio_ctrl = ioremap(GPIO14_CTRL, 0x4);
    pad_ctrl  = ioremap(PADS14_CTRL, 0x4);
    // read and set pdctrl at first
    pdctrl = ioread32(pad_ctrl);
    pdctrl &= ~BIT_PADS_OD;
    pdctrl |=  BIT_PADS_IE;
    iowrite32(pdctrl, pad_ctrl);
    // set ioctrl
    ioctrl = ioread32(gpio_ctrl);
    bak = ioctrl;
    ioctrl |= (BIT_IO_OEOVER | BIT_IO_OUTOVER);
    iowrite32(ioctrl, gpio_ctrl);

    return 0;
}
module_init(rgb_led_init);

static void __exit rgb_led_exit(void)
{
    // led off
    iowrite32(bak, gpio_ctrl);
    iounmap(gpio_ctrl);
    iounmap(pad_ctrl);
}
module_exit(rgb_led_exit);
