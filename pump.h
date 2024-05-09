/* 
 * pump.h * dummy pump controller simulator - pinouts
 */ 

#ifndef _PUMP_H
#define _PUMP_H

/* number of digits for mesures */
#define DEVICE_NAME "pump"
#define CLASS_NAME "pump_class"

#define MESURE_SIZE 2

//const char * devname = "pump";

/* struct to handle float mesures */
struct float_m {
	char decimal;
	char floating;
} float_m;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

void pump_engage(void);
void pump_shutdown(void);
int get_pump_state(void);

void parse_mesure(struct float_m * m, char * out);
void get_pressure(void);
void get_humidity(void);
void get_temperature(void);

int open_sysfs_attr(struct kobject * mod);

#endif
