#include <linux/module.h>			/* This module defines functions such as module_init/module_exit */
#include <linux/platform_device.h> 	/* For platform device */
#include <linux/gpio/consumer.h> 	/* For GPIO Descriptor */
#include <linux/of.h> 				/* For DT */

#define DRIVER_AUTHOR "phonglt15 p.linuxfromscratch@gmail.com"
#define DRIVER_DESC   "LED blinking"

static struct gpio_desc *LED; //2. Khai bao led. Tuy nhien chua co thong tin -> probe -> device tree

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "bbb-led,dt", }, //1
    { /* sentinel */ }
};

static int led_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

    LED = gpiod_get_index(dev, "mled", 0, GPIOD_OUT_HIGH);
    gpiod_set_value(LED,1);

    pr_info("Hello! Driver probe successfully!\n");
    return 0;
}

static int led_remove(struct platform_device *pdev)
{
    gpiod_put(LED);
	
	pr_info("Good bye my fen !!!\n");
	return 0;
}

static struct platform_driver my_led_drv = { //dang ky danh sach cac driver
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
    .name = "bbb-led,dt_sample",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(my_led_drv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 

/*
    danh sach cac thiet bi
    1. su tuong thich -> tim kiem chuoi "bbb-led" trong tat ca cac file device tree
    Neu no tim thay mot truong compatible nao do cua mot cai node
    ex[node]
    		led2 {
			label = "beaglebone:green:usr0";
			gpios = <&gpio1 21 GPIO_ACTIVE_HIGH>;
			linux,default-trigger = "heartbeat";
			default-state = "off";
		};
    Moi mot node se dai dien cho mot phan cung <1 phan cung tuong ung vs 1 driver dieu khien
    phan cung day
    -> danh sach cac thiet bi
*/

/*
    mot ct phai co 2 danh sach:
        - danh sach cac thiet bi
        - danh sach cac driver
*/


/*
    danh sach cac driver
        -probe: dai dien khi device va driver compatible voi nhau
        -remove: loai bo ct
        -driver: Khai bao driver su dung cho he thong nao
            .name: 
            .owner: THIS_MODULE
            .of_match_table = of_match_ptr
*/

/*
    khi probe tuong thich voi driver
    gpiod = gpio descriptor
    gpiod_get_index <==> export
    gpiod_put <==> unexport
    gpiod_get <device, id, in/out>
    khi led_probe chay se tra ve mot struct platform_device va no dai dien cho driver mled
    Muon get device dung struct device 
    ten driver device tree
*/