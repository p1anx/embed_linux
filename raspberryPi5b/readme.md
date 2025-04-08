## raspberry pi 
### mirror source 
```bash
sudo sed 's/deb.debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list -i
sudo apt update
```
# send.sh

The script is applied to send the file from local to host.

```bash
#!/bin/bash
function send(){
    local FILE=$1
    local HOST="root@192.168.1.39"
    sudo rsync -av $FILE "$HOST:/home/neox/code"
    echo "=======file is sent======="
}
send $1

```
## compile
## local compile
1. install requirement
```bash
sudo apt install bc bison flex libssl-dev make -y
```
### cross compile
```bash
#!/bin/bash
KERNEL=kernel_2712
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- clean
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2712_defconfig
#make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- dtbs -j$(nproc)
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image modules dtbs -j$(nproc)


```

### install kernel to pi5, using `rsync`
- filename `install_kernel.sh`
```bash
#!/bin/bash 
KERNEL=kernel_2712
KERNEL_PATH=/home/xwj/0_code/linux-rpi/linux #determine the compiled kernel's directory
REMOTE_HOST="root@192.168.1.44" #determine the pi5 kernel's host
sudo rsync $KERNEL_PATH/arch/arm64/boot/Image $REMOTE_HOST:/boot/firmware/$KERNEL.img
sudo rsync $KERNEL_PATH/arch/arm64/boot/dts/broadcom/*.dtb $REMOTE_HOST:/boot/firmware/
sudo rsync $KERNEL_PATH/arch/arm64/boot/dts/overlays/*.dtb* $REMOTE_HOST:/boot/firmware/overlays/
sudo rsync $KERNEL_PATH/arch/arm64/boot/dts/overlays/README $REMOTE_HOST:/boot/firmware/overlays/

```
- filename `send.sh`
```bash
#!/bin/bash
function send(){
    local FILE=$1
    local HOST="root@192.168.1.44"
    rsync -av --info=progress2 $FILE "$HOST:/home/pi"
    echo "=======file is sent======="
}
send $1

```
- usage
./send.sh foo.ko
## To install the 64-bit kernel
```bash
sudo cp /boot/firmware/$KERNEL.img /boot/firmware/$KERNEL-backup.img
sudo cp arch/arm64/boot/Image.gz /boot/firmware/$KERNEL.img
sudo cp arch/arm64/boot/dts/broadcom/*.dtb /boot/firmware/
sudo cp arch/arm64/boot/dts/overlays/*.dtb* /boot/firmware/overlays/
sudo cp arch/arm64/boot/dts/overlays/README /boot/firmware/overlays/
```

