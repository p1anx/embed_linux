- use `ls /sys/bus/platform/drivers`  to check the `platform driver`

## for `/dev`
According to the following file, replace `of_find_node_by_path()` function with `pdev->dev.of_node` in the `platform driver`.

```c
static int led_gpio_init(struct platform_device *pdev){
    int ret;
    led_cdev.nd = pdev->dev.of_node;
    // led_cdev.nd = of_find_node_by_path(DTS_NODE);
    led_cdev.led_gpio = of_get_named_gpio(led_cdev.nd, "gpios", 0);
    gpio_request(led_cdev.led_gpio, "led_gpio21");
    ret = gpio_direction_output(led_cdev.led_gpio, 1); // 默认输出高电平
    return 0;
}
```