/* 
 * pump_sysfs.c * dummy pump controller simulator - sysfs interface
 *
 * get commands from userspace tool through /dev/pump device
 * show resulting action in kernel log
 * 
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

// sysfs representation of struct_m
static char * pressure_f;
static char * temperature_f;
static char * humidity_f;

// maybe use a structure
static ssize_t pressure_f_show(struct kobject *kobj, 
                               struct kobj_attribute *attr, char *buf) 
{ 
    return sprintf(buf, "%d\n", pressure_f); 
} 

static ssize_t pressure_f_store(struct kobject *kobj, 
                                struct kobj_attribute *attr, char *buf, 
                                size_t count) 
{ 
    sscanf(buf, "%du", &pressure_f);
    return count; 
} 

static ssize_t temperature_f_show(struct kobject *kobj, 
                               struct kobj_attribute *attr, char *buf) 
{ 
    return sprintf(buf, "%d\n", temperature_f); 
} 

static ssize_t temperature_f_store(struct kobject *kobj, 
                                struct kobj_attribute *attr, char *buf, 
                                size_t count) 
{ 
    sscanf(buf, "%du", &temperature_f);
    return count; 
}
static ssize_t humidity_f_show(struct kobject *kobj, 
                               struct kobj_attribute *attr, char *buf) 
{ 
    return sprintf(buf, "%d\n", humidity_f); 
} 

static ssize_t humidity_f_store(struct kobject *kobj, 
                                struct kobj_attribute *attr, char *buf, 
                                size_t count) 
{ 
    sscanf(buf, "%du", &humidity_f);
    return count; 
}

static struct kobj_attribute temperature_f_attribute = 
    __ATTR(temperature_f, 0660, temperature_f_show, (void *)temperature_f_store);
static struct kobj_attribute humidity_f_attribute = 
    __ATTR(humidity_f, 0660, humidity_f_show, (void *)humidity_f_store);
static struct kobj_attribute pressure_f_attribute = 
    __ATTR(pressure_f, 0660, pressure_f_show, (void *)pressure_f_store);

int open_sysfs_attr(struct kobject * mod) 
{
    int error = 0; 

    mod = kobject_create_and_add("pumpmodule", kernel_kobj); 

    if (!mod) 
        return -ENOMEM; 

    
    error = sysfs_create_file(mod, &temperature_f_attribute.attr);
    if (error) { 
        goto failed;
    } 

    error = sysfs_create_file(mod, &humidity_f_attribute.attr); 
    if (error) { 
        goto failed;
    }

    error = sysfs_create_file(mod, &pressure_f_attribute.attr);
    if (error) { 
        goto failed;
        
    } 

    failed:
        pr_info("failed to create the pumpmodule file " 
                "in /sys/kernel/pumpmodule\n"); 

    return error; 
}

MODULE_LICENSE("GPL");