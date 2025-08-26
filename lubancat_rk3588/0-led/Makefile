MODULE_NAME := led
OVERLAY_NAME := $(MODULE_NAME)
HOST_IP := root@192.168.1.140
KERNEL_DIR := /home/rk/lubancat_sdk/kernel-5.10
KERNEL_VERSION := 5.10.209-rk3588
APP := ledapp

# 交叉编译设置
ARCH := arm64
CROSS_COMPILE := aarch64-none-linux-gnu-
export ARCH CROSS_COMPILE

# 内核模块对象
obj-m := $(MODULE_NAME).o

# 安装路径
INSTALL_TO_MODULE_DIR := $(HOST_IP):/lib/modules/$(KERNEL_VERSION)/
INSTALL_TO_OVERLAY_DIR := $(HOST_IP):/boot/dtb/overlay/
INSTALL_TO_UENV_TXT_DIR := $(HOST_IP):/boot/uEnv/
INSTALL_TO_APP_DIR := $(HOST_IP):~

# 当前路径
CURRENT_PATH := $(shell pwd)

# 默认目标
.PHONY: all build kernel_modules install install_dtbo dtbo app clean help install_app

all: kernel_modules dtbo app

# 编译内核模块
build: kernel_modules

kernel_modules:
        $(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules

# 安装内核模块
install: kernel_modules
        @echo "Installing kernel module..."
        sudo rsync -av $(MODULE_NAME).ko $(INSTALL_TO_MODULE_DIR)
        @echo "Running depmod on target..."
        ssh $(HOST_IP) "depmod -a"

# 安装设备树覆盖层
install_dtbo: dtbo
        @echo "Installing device tree overlay..."
        sudo rsync -av $(OVERLAY_NAME).dtbo $(INSTALL_TO_OVERLAY_DIR)
        @if [ -f uEnv.txt ]; then \
                echo "Installing uEnv.txt..."; \
                sudo rsync -av uEnv.txt $(INSTALL_TO_UENV_TXT_DIR); \
        else \
                echo "Warning: uEnv.txt not found"; \
        fi
# dts头文件预处理
CPP := $(CROSS_COMPILE)cpp
CPP_FLAGS := -nostdinc -I $(KERNEL_DIR)/include -undef -x assembler-with-cpp
# 编译设备树覆盖层
dtbo: $(OVERLAY_NAME).dtbo

# $(OVERLAY_NAME).dtbo: $(OVERLAY_NAME).dts
#         @echo "Compiling device tree overlay..."
#         dtc -@ -I dts -O dtb -o $@ $<
$(OVERLAY_NAME).dtbo: $(OVERLAY_NAME).dts
        @echo "Preprocessing and compiling device tree overlay..."
        $(CPP) $(CPP_FLAGS) $< > $<.preprocessed
        dtc -@ -I dts -O dtb -o $@ $<.preprocessed
        @echo "Cleaning up preprocessed file..."
        rm -f $<.preprocessed

# 编译用户空间应用
app: $(APP)

$(APP): $(APP).c
        @echo "Compiling application..."
        $(CROSS_COMPILE)gcc -o $@ $<

install_app: $(APP)
                sudo rsync -av $(APP) $(INSTALL_TO_APP_DIR)
# 完整安装
install_all: install install_dtbo install_app
        @echo "All components installed successfully"

# 清理
clean:
        $(MAKE) -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
        rm -f $(OVERLAY_NAME).dtbo $(APP)
        @echo "Clean completed"

# 检查依赖
check:
        @echo "Checking build environment..."
        @echo "KERNEL_DIR: $(KERNEL_DIR)"
        @echo "ARCH: $(ARCH)"
        @echo "CROSS_COMPILE: $(CROSS_COMPILE)"
        @echo "HOST_IP: $(HOST_IP)"
        @which $(CROSS_COMPILE)gcc > /dev/null || echo "Warning: Cross compiler not found"
        @test -d $(KERNEL_DIR) || echo "Warning: Kernel directory not found"
        @test -f $(OVERLAY_NAME).dts || echo "Warning: DTS file not found"
        @test -f $(APP).c || echo "Warning: Application source not found"

# 远程操作
remote_load:
        @echo "Loading module on target..."
        ssh $(HOST_IP) "modprobe $(MODULE_NAME)"

remote_unload:
        @echo "Unloading module on target..."
        ssh $(HOST_IP) "modprobe -r $(MODULE_NAME)"

remote_status:
        @echo "Checking module status..."
        ssh $(HOST_IP) "lsmod | grep $(MODULE_NAME) || echo 'Module not loaded'"

# 帮助信息
help:
        @echo "Available targets:"
        @echo "  all           - Build kernel module, dtbo, and app"
        @echo "  build         - Build kernel module only"
        @echo "  dtbo          - Compile device tree overlay"
        @echo "  app           - Compile user application"
        @echo "  install       - Install kernel module"
        @echo "  install_dtbo  - Install device tree overlay"
        @echo "  install_all   - Install everything"
        @echo "  clean         - Clean build files"
        @echo "  check         - Check build environment"
        @echo "  remote_load   - Load module on target"
        @echo "  remote_unload - Unload module on target"
        @echo "  remote_status - Check module status on target"
        @echo "  help          - Show this help"
