# add the following code to .dts
```dts 
/{
  myled{
    #address-cells = <1>;
    #size-cells = <1>;
    status = "okay";
    compatible = "xwj-myled";
    pinctrl-name = "default";
    pinctrl-0 = <&pinctrl_gpioled>;
    led-gpio = <&gpio1 3 GPIO_ACTIVE_LOW>;
  
};
};

&iomuxc{
    pinctrl-name = "default";
    pinctrl-0 = <&pinctrl_hog_1>;
    imx6u-evk{
      pinctrl_gpioled: ledgrp{
      fsl,pins = <
        MX6UL_PAD_GPIO1_IO3__GPIO1_IO3 0x10b0
      >;
      };
    };
};
```

```bash
make dtbs
```
check the node availablily
```bash
ls /proc/device-tree
```
