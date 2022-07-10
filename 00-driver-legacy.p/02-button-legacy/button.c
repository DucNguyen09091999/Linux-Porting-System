#include <linux/module.h>		/* This module defines functions such as module_init/module_exit */
#include <linux/gpio.h> 		/* For Legacy integer based GPIO */
#include <linux/interrupt.h>	/* For IRQ */
#include <linux/delay.h>		/* */
#include "button.h"
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>

#define DRIVER_AUTHOR "Ducnguyen nguyenchiduc090999@gmail.com"
#define DRIVER_DESC   "On/Off led with interrupt button"
#define LED_ON      _IOR('a', '1', int32_t *)
#define LED_OFF     _IOW('a', '0', int32_t *)
#define CDEV_NAME "m-cdev"
#define CLASS_NAME "m-class"
#define DEVICE_NAME "m-device"
#define DEV_MEM_SIZE 1024
#define NPAGES  1
struct m_foo_dev{   
    int size;
    dev_t dev_num;
    struct cdev m_cdev;
    struct class *m_class;
}mdev;

static int irq;
uint32_t __iomem *base_addr;
volatile int32_t state;



static int __init m_btn_irq_init(void);
static void __exit m_btn_irq_exit(void);
static long m_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);

static struct file_operations m_fops ={
    .owner      = THIS_MODULE,
    .unlocked_ioctl = m_ioctl,
};

static long m_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
    switch(cmd) {
        case LED_ON:
            pr_info("LED on\n");
	        *(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= LED_PIN;
            break;

        case LED_OFF:
            pr_info("LED off\n");
            *(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED_PIN;
            break;

        default:
            pr_info("Default\n");
            break;
    }

    return 0;
}


static irq_handler_t m_btn_irq_handle(unsigned int irq, void *dev_id, struct pt_regs *regs){
  	state = gpio_get_value(LED_PIN);
	if (state == 0) {		
		*(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= (1 << LED_PIN);
		state = 1;
	} else {
		*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= (1 << LED_PIN);
		state = 0;
	}
	return (irq_handler_t) IRQ_HANDLED;
}

static int __init m_btn_irq_init(void){	
	uint8_t ret_val;

	        //1.0 dynamic allocate a device number
    if((alloc_chrdev_region(&mdev.dev_num,0,1,CDEV_NAME)) < 0){
        pr_err("Can't allocate chrdev region\n");
        return -1;
    }
    pr_info("[INFO] Major = %d, Minor = %d\n",MAJOR(mdev.dev_num),MINOR(mdev.dev_num));

        //1.1 Initialize cdev structure with fops
    cdev_init(&mdev.m_cdev,&m_fops);
    
        //1.2 Register cdev structure with VFS
        //mdev.m_cdev.owner = THIS_MODULE;
    if(cdev_add(&mdev.m_cdev,mdev.dev_num,1) < 0){
        pr_err("Can't register cdev structure to the system\n");
        goto rm_dev_numb;
    }

        //1.3 Create device class under /sys/class/
    if((mdev.m_class = class_create(THIS_MODULE,CLASS_NAME)) == NULL){
        pr_err("Can't create device class for my device\n");
        goto rm_dev_numb;
    }

        //1.4 Creating device
    if((device_create(mdev.m_class,NULL,mdev.dev_num,NULL,DEVICE_NAME)) == NULL){
        pr_err("Can't create my device\n");
        goto rm_class;
    }
	
	/* Config LED as output mode*/
	base_addr = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
	if(!base_addr)
		return -ENOMEM;


	*(base_addr + GPIO_OE_OFFSET / 4) &= ~(1 << LED_PIN);
	*(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= (1 << LED_PIN);

	gpio_direction_input(BUTTON_PIN);        // Set the button GPIO to be an input
	gpio_set_debounce(BUTTON_PIN, 200); // Debounce the button with a delay of 200ms
	gpio_export(BUTTON_PIN, false);

	irq = gpio_to_irq(BUTTON_PIN);	// The interrupt number requested 
	ret_val = request_irq(irq,(irq_handler_t) m_btn_irq_handle,IRQF_TRIGGER_FALLING,"m-button",NULL);                             
	pr_info("On/Off led using button program loaded\n");
	return 0;

    rm_class:
        class_destroy(mdev.m_class);
    rm_dev_numb:
        unregister_chrdev_region(mdev.dev_num,1);
        return -1;

}

static void
__exit m_btn_irq_exit(void){	
	*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= (1 << LED_PIN); 
	free_irq(irq, NULL);
	iounmap(base_addr);

	//1.3 Destroy device
    device_destroy(mdev.m_class,mdev.dev_num);
    //1.2 Destroy class
    class_destroy(mdev.m_class);
    //1.1 Destroy cdev
    cdev_del(&mdev.m_cdev);
    //1.0 Destroy allocate chrdev region
    unregister_chrdev_region(mdev.dev_num,1);

	pr_info("On/Off led using button program unloaded\n");
}

module_init(m_btn_irq_init);
module_exit(m_btn_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
MODULE_INFO(board, "Beaglebone Black Rev.C");
