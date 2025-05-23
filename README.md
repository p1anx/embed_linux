
# embed_linux

# Install cross compile toolchain
## method 1
### for 64-bit
```bash
sudo apt install bc bison flex libssl-dev make libc6-dev libncurses5-dev -y
sudo apt install crossbuild-essential-arm64 -y
```
### for 32-bit

```bash
sudo apt install bc bison flex libssl-dev make libc6-dev libncurses5-dev -y
sudo apt install crossbuild-essential-armhf -y
```

## for imx6ull
###
arm gcc 7.5, 6.5 and 5.5 are not ok, 4.9 is ok
```bash
#!/bin/bash
sudo apt install build-essential -y 
sudo apt install libncurses5-dev -y
sudo apt install gcc-9 -y 
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 2 

wget "https://releases.linaro.org/components/toolchain/binaries/4.9-2017.01/arm-linux-gnueabihf/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar.xz"
if [ ! -f "gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar.xz" ];then
  echo "failed to download gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar.xz"
  exit 1
fi
tar -xvf gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar.xz
sudo cp -r gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf /usr/local
if [ -f $HOME/.bashrc ];then
  tee -a $HOME/.bashrc << EOF
export PATH=/usr/local/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin:$PATH
EOF
  echo "bashrc is configured for arm-gcc"
fi

if [ -f $HOME/.config/fish/config.fish ];then
  tee -a $HOME/.config/fish/config.fish << EOF
set PATH /usr/local/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin $PATH
EOF
  echo "config.fish is configured for arm-gcc"
fi


```
### Install TFTP
```bash
#!/bin/bash
TFTP_DIR=$HOME/embed_linux/tftp

sudo apt install xinetd -y
sudo apt install tftp-hpa tftpd-hpa -y

mkdir -p $TFTP_DIR
sudo chmod 777 $TFTP_DIR

if [ -f "/etc/default/tftpd-hpa" ];then
  sudo sed -i "s#TFTP_DIRECTORY=\"/srv/tftp\"#TFTP_DIRECTORY=\"${TFTP_DIR}\"#g" /etc/default/tftpd-hpa
else
  echo "tftpd-hpa file does not exist"
fi

if [ ! -f "/etc/xinetd.d/tftp" ];then
  sudo touch /etc/xinetd.d/tftp
  sudo tee /etc/xinetd.d/tftp << EOF
server tftp
{
  socket_type = dgram
  wait = yes
  disable = no
  user = root
  protocol = udp
  server = /usr/sbin/in.tftpd
  server_args = -s ${TFTP_DIR} -c
#log_on_success += PID HOST DURATION
#log_on_failure += HOST
  per_source = 11
  cps =100 2
  flags =IPv4
}
EOF
fi

sudo service tftpd-hpa restart
sudo service xinetd restart

```

## Install NFS
```bash
#!/bin/bash
NFS_DIR=$HOME/embed_linux/nfs

sudo apt install nfs-kernel-server -y 
sudo mkdir -p $NFS_DIR
sudo chmod 777 $NFS_DIR

if [ -f "/etc/exports" ];then
  sudo tee -a /etc/exports << EOF
${NFS_DIR} *(rw,sync,no_root_squash)
EOF
else
  echo "/etc/exports file does not exist"
  echo "nfs config is failed"
fi

sudo /etc/init.d/nfs-kernel-server restart
showmount -e
```

### make linux 
```bash
#!/bin/bash 
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- imx_alientek_emmc_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j$(nproc)
```

## raspberry pi 
### mirror source 
```bash
sudo sed 's/deb.debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list -i
sudo apt update
```


## VSCode c includepath
for arm
```json
"linux/include",
"linux/arch/arm/include",
"linux/arch/arm/include/generated",
```

for arm64
```json
"/home/xwj/0_code/linux-rpi/linux/include",
"/home/xwj/0_code/linux-rpi/linux/arch/arm64/include",
"/home/xwj/0_code/linux-rpi/linux/arch/arm64/include/generated",
```

## cdev
## check
cdev.ko ------> lsmod | grep cdev
```c 
major  = register_chrdev(0, "hello_cdev", &fops); // cat /proc/devices | grep hello_cdev ----> 508 cdev(508 is major)
```

```bash
sudo mknod /dev/hello0 c 508 0 (create major and minor)
```



## Makefile
### cross compile
```Makefile
    KERNEL_DIR=../ebf-buster-linux/build_image/build
    ARCH=arm
    CROSS_COMPILE=arm-linux-gnueabihf-
    export  ARCH  CROSS_COMPILE

    obj-m := led_cdev.o
    out =  led_cdev_test

    all:
            $(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
            $(CROSS_COMPILE)gcc -o $(out) led_test.c

    .PHONE:clean copy

    clean:
            $(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean
            rm $(out)
```

### natively compile
```Makefile
obj-m += mod_auto_cdev.o

all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```
