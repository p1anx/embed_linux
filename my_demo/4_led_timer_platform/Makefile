obj-m := led_timer_platform.o

KERNEL_DIR := /home/xwj/0_code/linux-rpi/linux

build: kernel_modules

kernel_modules:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KERNEL_DIR) M=$(PWD) clean
