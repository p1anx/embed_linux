#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
// #include <linux/leds_pwm.h>  // 删除这行，因为代码中没有使用
#include <linux/slab.h>

struct pwm_device	*pwm_demo;  //定义pwm设备结构体

/*简单驱动*/
static int pwm_demo_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device *dev = &pdev->dev;
	struct device_node *child; // 保存子节点
	/*使用新的PWM API配置参数*/
	struct pwm_state state;
	printk("match success \n");
	
	child = of_get_next_child(dev->of_node, NULL);
	if (child)
	{
		pwm_demo = devm_of_pwm_get(dev, child, NULL);
		if (IS_ERR(pwm_demo)) 
		{
			printk(KERN_ERR" pwm_demo，get pwm error!!\n");
			return -1;
		}
	}
	else
	{
		printk(KERN_ERR" pwm_demo of_get_next_child error!!\n");
		return -1;
	}
	
	
	// 获取当前PWM状态
	pwm_get_state(pwm_demo, &state);
	
	// 配置PWM参数：频率200KHz (5000ns周期)，占空比20% (1000ns)，反相
	state.period = 5000;      // 5000ns = 200KHz
	state.duty_cycle = 2500;  // 1000ns = 20% duty cycle
	state.polarity = PWM_POLARITY_INVERSED;  // 反相
	state.enabled = true;     // 使能PWM
	
	// 应用PWM配置
	ret = pwm_apply_state(pwm_demo, &state);
	if (ret < 0) {
		printk(KERN_ERR "pwm_demo: failed to apply PWM state\n");
		return ret;
	}
	
	return ret;
}

static int pwm_demo_remove(struct platform_device *pdev)
{
	struct pwm_state state;
	
	// 禁用PWM
	pwm_get_state(pwm_demo, &state);
	state.enabled = false;
	pwm_apply_state(pwm_demo, &state);
	
	return 0;
}

static const struct of_device_id of_pwm_leds_match[] = {
	{.compatible = "pwm_demo",},
	{},
};

static struct platform_driver pwm_demo_driver = {
	.probe		= pwm_demo_probe,
	.remove		= pwm_demo_remove,
	.driver		= {
		.name	= "pwm_demo",
		.of_match_table = of_pwm_leds_match,
	},
};

module_platform_driver(pwm_demo_driver);

MODULE_AUTHOR("llh <zgwinli555@163.com>");
MODULE_DESCRIPTION(" Embedfire PWM DEMO driver");
MODULE_LICENSE("GPL v2");
