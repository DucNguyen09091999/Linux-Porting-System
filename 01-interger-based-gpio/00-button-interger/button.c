#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define DRIVER_AUTHOR "DucNguyen ducnguyen090999@gmail.com"
#define DRIVER_DESCRIPTION "blink led using interger base gpio"


#define GPIO_Number(bank,pin) (32*bank + pin)
#define GPIO_LED GPIO_Number(0,30)
static int
__init m_inter_base_GPIO_init(void){
    gpio_request(GPIO_LED,"led");
    gpio_direction_output(GPIO_LED,0);
    gpio_set_value(GPIO_LED,1);

    pr_info("Blink led loaded\n");
    return 0;
}


static void
__exit m_inter_base_GPIO_exit(void){
    gpio_set_value(GPIO_LED,0);
    gpio_free(GPIO_LED);
    pr_info("blink led unloaded\n");
}


module_init(m_inter_base_GPIO_init);
module_exit(m_inter_base_GPIO_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION("1.0");