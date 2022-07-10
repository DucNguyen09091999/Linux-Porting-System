#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/string.h>



#define DRIVER_AUTH "DucNguyen nguyenchiduc090999@gmail.com"
#define DRIVER_DESC "Create kobject in /sys"

struct m_private_dev{
    struct kobject *kobj_ref; //reference
}m_dev;


//Function prototype
static int __init sysfs_direction_init(void);
static void __exit sysfs_direction_exit(void);

//Sysfs function prototype
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count);
struct kobj_attribute value = __ATTR(value,0660,value_show,value_store);
struct kobj_attribute direction = __ATTR(direction,0660,direction_show,direction_store);

//Function action

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    pr_info("value read.....\n");

    return 0;
}

/* This function will be called when we write the sysfsfs file */
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
    pr_info("value write.....\n");

    return count;
}

/* This function will be called when we read the sysfs file */
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    pr_info("direction read.....\n");

    return 0;
}

/* This function will be called when we write the sysfsfs file */
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
    pr_info("direction write.....\n");

    return count;
}


//Initialize group attrs
static struct attribute *attrs[] = {
    &direction.attr,
    &value.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static int 
__init sysfs_direction_init(void){
    //Create kobject in /sys
    m_dev.kobj_ref = kobject_create_and_add("bb-gpios",NULL);
    //Create entry under /sys/bb-gpios/
    if(sysfs_create_group(m_dev.kobj_ref,&attr_group)){
        pr_err("Can't create system file\n");
        goto rm_kobj;
    }


    pr_info("Hello. Initialized successfully\n");
    return 0;

    rm_kobj:
        kobject_put(m_dev.kobj_ref);
        return -1;
}
static void
__exit sysfs_direction_exit(void){
    sysfs_remove_group(m_dev.kobj_ref,&attr_group);
    kobject_put(m_dev.kobj_ref);

    pr_info("Goodbye\n");
}

module_init(sysfs_direction_init);
module_exit(sysfs_direction_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTH);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
MODULE_INFO(board,"Beaglebone Black Rev.C");