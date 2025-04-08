#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x122c3a7e, "_printk" },
	{ 0xf3ebda75, "__platform_driver_register" },
	{ 0x82ee90dc, "timer_delete_sync" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0xcf825a00, "platform_driver_unregister" },
	{ 0xfe990052, "gpio_free" },
	{ 0x9875d727, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe3d97303, "device_destroy" },
	{ 0xf4e1b5d, "class_destroy" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x282c842f, "cdev_init" },
	{ 0xe72ec58e, "cdev_add" },
	{ 0x1b58158e, "class_create" },
	{ 0xcc4889d4, "device_create" },
	{ 0xc2527e16, "of_get_named_gpio" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xcc3f7cf8, "gpio_to_desc" },
	{ 0x250dd165, "gpiod_direction_output_raw" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x24d273d1, "add_timer" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xf5ea2d74, "gpiod_set_raw_value" },
	{ 0xdcb764ad, "memset" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x47e64c59, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B2F445F8F2413CEAC32A297");
