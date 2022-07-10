#include "blink_led.h"
#include <linux/module.h>
#include <linux/io.h> //ioremap() iounmap()
#include <linux/kernel.h>


#define DRIVER_AUTHOR "DucNguyen nguyenchiduc090999@gmail.com"
#define DRIVER_DESC "Blink led program"

uint32_t __iomem *base_addr;

static int
__init led_init(void){
    base_addr = ioremap(GPIO_0_ADDR_BASE,GPIO_0_ADDR_SIZE); //return maped memory
    if(!base_addr){
        return -ENOMEM;
    }

    *(base_addr +GPIO_OE_OFFSET/4) &= ~LED;
    *(base_addr + GPIO_SETDATAOUT_OFFSET/4) |= LED;

    pr_info("Hello! Initialized successfully\n");
    return 0;
}

static void
__exit led_exit(void){
    *(base_addr + GPIO_CLEARDATAOUT_OFFSET/4) |= LED;
    iounmap(base_addr);

    pr_info("Goodbye my friend\n");
}


module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");