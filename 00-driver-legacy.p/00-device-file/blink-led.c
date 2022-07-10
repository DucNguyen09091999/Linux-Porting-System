#include <linux/module.h> //allocate
#include <linux/fs.h> //major minor
#include <linux/device.h> //class -device
#include <linux/cdev.h>
#undef pr_fmt
#define pr_fmt(fmt)"%s : " fmt,__func__

#define DRIVER_AUTH "DucNguyen nguyenchiduc090999@gmail.com"
#define DRIVER_DESC "Blink led driver"


#define CDEV_NAME "m-cdev"
#define CLASS_NAME "m-class"
#define DEVICE_NAME "m-device"
struct m_drv_private_data
{
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
}m_drv_data;


static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size,loff_t * offset);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = m_read,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};

/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call read() called...!!!\n");
    return 0;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call write() called...!!!\n");
    return size;


static int
__init m_blink_led_driver_init(void){
    //1.1 dynamic allocation chrdev region
    if(alloc_chrdev_region(&m_drv_data.dev_num,0,1,CDEV_NAME) < 0) {
        pr_err("Failed to allocate chrdev region\n");
        return -1;
    }
    pr_info("Major = %d, Minor = %d\n",MAJOR(m_drv_data.dev_num), \
                                        MINOR(m_drv_data.dev_num));
    //1.2 Create cdev
    cdev_init(&m_drv_data.m_cdev,&m_fops);

    if(cdev_add(&m_drv_data.m_cdev,m_drv_data.dev_num,1) < 0){
        pr_err("Failed to add cdev ")
    }
    //2.1 Create class
    if((m_drv_data.m_class = class_create(THIS_MODULE,CLASS_NAME)) == NULL){
        pr_err("Failed to create class for my device\n");
        goto rm_dev_numb;
    }
    //3.1 Device create
    if(device_create(m_drv_data.m_class,NULL,m_drv_data.dev_num,NULL,DEVICE_NAME) == NULL){
        pr_err("Failed to create my device\n");
        goto rm_class;
    }
    pr_info("Blink led driver loaded\n");
    return 0;
rm_class:
    class_destroy(m_drv_data.m_class);
rm_dev_numb:
    unregister_chrdev_region(m_drv_data.dev_num,1);
    return -1;

}

static void
__exit m_blink_led_driver_exit(void){
    //3.1 Destroy device
    device_destroy(m_drv_data.m_class,m_drv_data.dev_num);

    //2.1 Destroy class
    class_destroy(m_drv_data.m_class);

    //1.1 Destroy allocate chrdev 
    unregister_chrdev_region(m_drv_data.dev_num,1);
    pr_info("Blink led driver unloaded\n");

}


module_init(m_blink_led_driver_init);
module_exit(m_blink_led_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTH);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
MODULE_INFO(board, "Beaglebone black Rev.C");
