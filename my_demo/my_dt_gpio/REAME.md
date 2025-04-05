## dts content
for raspberry pi 5b, add the following content to 'bcm2712-rpi-5-b.dts' file
`gpios = <&rp1_gpio 21 0>` is set to configure gpio 21.
note: for pi5, we need to write `rp1_gpio`
```dts 
  ledtest: ledtest{
    compatible = "ledtest";
    pinctrl-names = "default";
    gpios = <&rp1_gpio 21 0>;
    status = "okay";
  };

```

## change the content in `mod_dt_gpio.c` file
change the `/ledtest` to your setup in `.dts` file
```c
test_device_node = of_find_node_by_path("/ledtest");
```

## compile

compile the `.dts` file and send to `/boot/firmware/` directory in pi5b
compile the `mod_dt_gpio.c` and `mainApp.c` to get `.ko` and `mainApp`, then send it to pi5b
```bash
sudo insmod mod_dt_gpio.ko
sudo ./mainApp 0 # 0: close led; 1: open led
```
