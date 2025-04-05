KERNEL_DIR := /home/xwj/0_code/linux-rpi/linux
CURRENT_PATH := $(shell pwd)

obj-m := mod_dt_gpio.o 

build: kernel_modules

kernel_modules:
	$(MAKE) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules 
clean:
	$(MAKE) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
