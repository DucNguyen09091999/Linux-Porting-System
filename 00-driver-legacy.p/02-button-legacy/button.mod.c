#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x64511672, "module_layout" },
	{ 0x49fa8385, "cdev_del" },
	{ 0x8c027195, "device_destroy" },
	{ 0xedc03953, "iounmap" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xc6ad2a45, "gpiod_to_irq" },
	{ 0x7352780f, "gpiod_export" },
	{ 0xeaeb4dff, "gpiod_set_debounce" },
	{ 0x8cb972e9, "gpiod_direction_input" },
	{ 0xe97c4103, "ioremap" },
	{ 0xcf9e4e60, "class_destroy" },
	{ 0x88cae38, "device_create" },
	{ 0x1305450c, "__class_create" },
	{ 0xffc33b5c, "cdev_add" },
	{ 0x38d114dd, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x88afe3ff, "gpiod_get_raw_value" },
	{ 0xddc96cf5, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "07E72C17C98C09DB3B6E3D9");
