/* 
 * pump.c * dummy pump controller simulator
 *
 * TODO: create command to fetch/write each parameter (IOCTL)
 *       read from /sys/kernel/pumpmodule
 *       
 */ 

#include <linux/atomic.h> 
#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/kernel.h> /* for sprintf() */ 
#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/types.h> 
#include <linux/uaccess.h> /* for get_user and put_user */ 
#include <linux/version.h> 
#include <asm/errno.h> 
#include "pump.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zyx");
MODULE_DESCRIPTION("Waterpump and sensors control module");
MODULE_VERSION("0.1");

int pump_state = 0;
/*double pressure = 0;
double humidity = 0;
double celsius = 0;*/

struct float_m pressure;
struct float_m humidity; 
struct float_m temperature;

// sysfs related
static int sysfsDesc;
static struct kobject * pumpmodule = NULL;

// device related
static int majorNumber;
static struct class* pumpClass = NULL;
static struct device* pumpDevice = NULL;

static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

void pump_engage(void)
{
	if (pump_state == 0) pump_state = 1;
}

void pump_shutdown(void)
{
	pump_state = 0;
}

int get_pump_state(void)
{
	return pump_state;
}

void parse_mesure(struct float_m * m, char * out)
{
    /* format mesure 00.00 */
	strcpy(out, m->decimal);
	strncat(out, m->floating, sizeof(char)*2);
}

void get_pressure(void)
{	
	char * out = kmalloc((sizeof(char) * MESURE_SIZE * 2), GFP_KERNEL);
	parse_mesure(&pressure, out);
	// write to device value
	kfree(out);
}

void get_humidity(void)
{
	char * out = kmalloc((sizeof(char) * MESURE_SIZE * 2), GFP_KERNEL);
	parse_mesure(&humidity, out);
	// write to device value
	kfree(out);
}

void get_temperature(void)
{
	char * out = kmalloc((sizeof(char) * MESURE_SIZE * 2), GFP_KERNEL);
	parse_mesure(&temperature, out);
	// write to device value
	kfree(out);
}

static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "Pump : Périphérique ouvert\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    printk(KERN_INFO "Pump : Tentative de lecture depuis le périphérique\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    printk(KERN_INFO "Pump : Tentative d'écriture dans le périphérique\n");
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "Pump : Périphérique fermé\n");
    return 0;
}

static int __init pump_module_init(void) 
{
 // Enregistrer le numéro majeur
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0)
    {
        printk(KERN_ALERT "Pump : Impossible de s'inscrire au périphérique caractère avec le numéro majeur %d\n", majorNumber);
        return majorNumber;
    }
    printk(KERN_INFO "Pump : Module de périphérique caractère enregistré avec le numéro majeur %d\n", majorNumber);

    // Enregistrer la classe du périphérique
    pumpClass = class_create(DEVICE_NAME);
    if (IS_ERR(pumpClass))
    {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Pump : Échec de la création de la classe du périphérique\n");
        return PTR_ERR(pumpClass);
    }
    printk(KERN_INFO "Pump : Classe du périphérique créée avec succès\n");

    // Enregistrer le périphérique
    pumpDevice = device_create(pumpClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(pumpDevice))
    {
        class_destroy(pumpClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Pump : Échec de la création du périphérique\n");
        return PTR_ERR(pumpDevice);
    }

    sysfsDesc = open_sysfs_attr(pumpmodule);

    printk(KERN_INFO "Pump : Périphérique créé avec succès\n");
    return 0;
}

static void __exit pump_module_exit(void)
{
    device_destroy(pumpClass, MKDEV(majorNumber, 0));
    class_unregister(pumpClass);
    class_destroy(pumpClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    
    kobject_put(pumpmodule); 

    printk(KERN_INFO "Pump : Module de périphérique caractère déchargé\n");
}

module_init(pump_module_init);
module_exit(pump_module_exit);

