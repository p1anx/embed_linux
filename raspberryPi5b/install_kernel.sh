#!/bin/bash
KERNEL=kernel_2712
KERNEL_PATH=/home/xwj/0_code/linux-rpi/linux
REMOTE_HOST="root@192.168.1.44"
sudo rsync $KERNEL_PATH/arch/arm64/boot/Image $REMOTE_HOST:/boot/firmware/$KERNEL.img
echo "Image is sent over"
sudo rsync $KERNEL_PATH/arch/arm64/boot/dts/broadcom/*.dtb $REMOTE_HOST:/boot/firmware/
echo "dtb is sent over"
sudo rsync $KERNEL_PATH/arch/arm64/boot/dts/overlays/*.dtb* $REMOTE_HOST:/boot/firmware/overlays/
echo "overlays dtb is sent over"
sudo rsync $KERNEL_PATH/arch/arm64/boot/dts/overlays/README $REMOTE_HOST:/boot/firmware/overlays/
echo "README is sent over"

