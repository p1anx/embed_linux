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
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xdcb764ad, "memset" },
	{ 0xf5ea2d74, "gpiod_set_raw_value" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x47e64c59, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "568C64EBD247178C59B0FCD");
