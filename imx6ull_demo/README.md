## 
kernel version : 4.1.15

## location of `.dtb` and `zImage` file
`arch/arm/boot/zImage`
`arch/arm/boot/dts/imx6ull-alientek-emmc.dtb`

## check the `device-tree` 
```bash

ls /proc/device-tree

```

## Usage
- add module
```bash
insmod led.ko
# or
depmod
modprobe led.ko

```
- check the module
```bash
lsmod
```
- remove the module
```bash
rmmod led.ko
```
- check the load info 
```bash
dmesg | tail
```

