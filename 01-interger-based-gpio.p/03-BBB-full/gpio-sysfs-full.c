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

//static int32_t _export;
static char gpioName[8] = {};
static int32_t GPIO_LED;
static int32_t _value = 0;
static char _direct[8] = "out"; //hold direction data [in,out]


struct m_private_dev{
    struct kobject *kobj_ref; //reference
}m_dev,mdev;


//Function prototype
static int __init sysfs_direction_init(void);
static void __exit sysfs_direction_exit(void);


//Sysfs function prototype
static ssize_t export_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t export_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count);
static ssize_t unexport_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t unexport_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count);

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count);


struct kobj_attribute value = __ATTR(value,0660,value_show,value_store);
struct kobj_attribute direction = __ATTR(direction,0660,direction_show,direction_store);
struct kobj_attribute export = __ATTR(export,0660,export_show,export_store);
struct kobj_attribute unexport = __ATTR(unexport,0660,unexport_show,unexport_store);

static struct attribute *attr[] = {
    &direction.attr,
    &value.attr,
    NULL,
};

static struct attribute_group attrs_group = {
    .attrs = attr,
};



static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int value;

    if(strcmp(_direct,"in") == 0){
        value = gpio_get_value(GPIO_LED);
        return sprintf(buf,"%d\n",value);
    }
    return sprintf(buf,"%d\n",_value); //get data from variable
      

}

/* This function will be called when we write the sysfsfs file */
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
    int32_t numb  = 0; 
    sscanf(buf, "%d", &numb); //get data from buf [buffer]. Then hole this data into number
    if(strcmp(_direct,"out") == 0){
    switch (numb) {
        case 0: /* off */
            gpio_set_value(GPIO_LED, 0);
            break;
        case 1: /* on */
            gpio_set_value(GPIO_LED, 1);
            break;

        default: //Immediately return if unwanted data
            return count;
    }
    }
    else {
        pr_info("Permission denied\n");
    }
    
    sscanf(buf, "%d", &_value); //save buf into value to value_show

    return count;
}

/* This function will be called when we read the sysfs file */
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", _direct); //get data from direct
}

/* This function will be called when we write the sysfsfs file */
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
    char pinstatus[8];
    sscanf(buf,"%s",pinstatus);
    if(strcmp(pinstatus,"out") == 0){
        sprintf(_direct,"%s",pinstatus);
        gpio_direction_output(GPIO_LED,_value);
    }
    else if(strcmp(pinstatus,"in") == 0){
        sprintf(_direct,"%s",pinstatus);
        gpio_direction_input(GPIO_LED);
    }
    else{
        return count;
    }


    return count;
}


static ssize_t export_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf){
    pr_info("Permission denied\n");
    return 0;
}

static ssize_t unexport_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf){
    pr_info("Permission denied\n");
    return 0;
}

static ssize_t export_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count){
    
    sscanf(buf,"%d",&GPIO_LED);//thu du lieu tu buf la mot so nguyen va truyen vao GPIO_LED
    
    if(GPIO_LED < 0 || GPIO_LED > 128){
        return count;
    } 
    else {
        
        snprintf(gpioName,8,"gpio%d",GPIO_LED);
        mdev.kobj_ref = kobject_create_and_add(gpioName,m_dev.kobj_ref);
        if(sysfs_create_group(mdev.kobj_ref,&attrs_group)){
        pr_err("Can't create system file\n");
        goto rm_kobj;
    }
        gpio_request(GPIO_LED,gpioName);
        gpio_direction_output(GPIO_LED,0);
    }



    //sscanf(buf,"%d",&_export);
    return count;

     rm_kobj:
        kobject_put(mdev.kobj_ref);
        return -1;
}


static ssize_t unexport_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count){
    
    int32_t numb = 0;
    sscanf(buf,"%d",&numb);
    snprintf(gpioName,8,"gpio%d",numb);
    
    gpio_free(GPIO_LED);
    sysfs_remove_group(mdev.kobj_ref,&attrs_group);
    kobject_put(mdev.kobj_ref);
    // strcpy(_direct,"out");
    // _value = 0;
    return count;
}

//Initialize group attrs
static struct attribute *attrs[] = {
    &export.attr,
    &unexport.attr,
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
    
    // sysfs_remove_group(mdev.kobj_ref,&attrs_group);
    // kobject_put(mdev.kobj_ref);

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
