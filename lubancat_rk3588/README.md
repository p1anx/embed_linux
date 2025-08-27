## overlays
###  install `dtc`
```bash
sudo apt install device-tree-compiler -y
```

### Usage
generate `led.dtbo`
```bash
dtc -@ -I dts -o dtb -o led.dtbo led.dts
```

1. add `led.dtbo` to `/boot/dtb/overlay` directory
2. location of `uEnv.txt` : /boot/uEnv/uEnv.txt
3. add the context to tail of `uEnv.txt`
```txt
dtoverlay=/dtb/overlay/led.dtbo
```
4. reboot to take effect


### overlay的dts中头文件处理
https://blog.csdn.net/dragon101788/article/details/105046227
```bash
cpp -nostdinc -I /home/rk/lubancat_sdk/kernel-5.10/include \
    -undef -x assembler-with-cpp input_key.dts > input_key_preprocessed.dts
# 然后编译预处理后的文件
dtc -@ -I dts -O dtb -o input_key.dtbo input_key_preprocessed.dts

```

### issues
#### 1
- GPIO6_A3 is used as input_button, it will show error for overlay

## rk_init
### rk_sys_init
```bash
#!/bin/bash
function rk_init() {
  apt update -y
  apt install vim git rsync fish -y
  sed -i "s/#PermitRootLogin prohibit-password/PermitRootLogin yes/g" /etc/ssh/sshd_config
  /etc/init.d/ssh restart

}
rk_init

```

## pwm 
pwm doesn't need app
