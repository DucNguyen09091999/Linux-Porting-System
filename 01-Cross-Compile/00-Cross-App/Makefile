#toolchain

TOOLCHAIN := /home/ducnguyen/Working-Space/beagle-bone-black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin
PREFIX := arm-linux-gnueabihf-


#ARCH

ifeq (${ARCH},arm)
	CC:= $(TOOLCHAIN)/$(PREFIX)gcc
endif
all:
	$(CC) -o hello hello.c
clean:
	rm -rf hello