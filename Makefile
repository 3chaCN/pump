obj-m += pumpi.o
pumpi-objs := pump_sysfs.o pump.o

PWD := $(CURDIR)  

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
