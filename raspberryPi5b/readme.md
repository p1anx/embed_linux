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
```bash

```
## To install the 64-bit kernel
```bash
sudo cp /boot/firmware/$KERNEL.img /boot/firmware/$KERNEL-backup.img
sudo cp arch/arm64/boot/Image.gz /boot/firmware/$KERNEL.img
sudo cp arch/arm64/boot/dts/broadcom/*.dtb /boot/firmware/
sudo cp arch/arm64/boot/dts/overlays/*.dtb* /boot/firmware/overlays/
sudo cp arch/arm64/boot/dts/overlays/README /boot/firmware/overlays/
```

