KDIR := /home/ducnguyen/Working-Space/beagle-bone-black/bb-kernel/KERNEL
CDIR := /home/ducnguyen/Working-Space/beagle-bone-black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin
PREFIX := arm-linux-gnueabihf-

#Host/pc
obj-m := gpio-sysfs-full.o
HDIR := /lib/modules/`uname -r`/build



all :
	make ARCH=arm CROSS_COMPILE=${CDIR}/${PREFIX} -C ${KDIR} M=$(PWD) modules


clean :
	make ARCH=arm CROSS_COMPILE=${CDIR}/${PREFIX} -C ${KDIR} M=$(PWD) clean

help :
	make ARCH=arm CROSS_COMPILE=${CDIR}/${PREFIX} -C ${KDIR} M=$(PWD) help
	
host:
	make -C $(HDIR) M=$(PWD) modules