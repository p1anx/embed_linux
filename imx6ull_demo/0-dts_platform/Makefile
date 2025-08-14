obj-m :=led_platform.o 

KERNEL_DIR := /root/imx_linux/kernel

CURRENT_PATH := $(shell pwd)


build: kernel_modules

kernel_modules:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules 
clean:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
