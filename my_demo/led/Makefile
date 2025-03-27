KERNEL_DIR := /home/xwj/embed_linux/imx-linux/alientek_linux
CURRENT_PATH := $(shell pwd)

obj-m := test_led.o 

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules 
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
