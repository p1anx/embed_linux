## keyApp control led by key
```c
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名          : keyApp.c
作者            : 左忠凯
版本            : V1.0
描述            : 按键输入测试应用程序
其他            : 无
使用方法         ：./keyApp /dev/key  
论坛            : www.openedv.com
日志            : 初版V1.0 2019/1/30 左忠凯创建
***************************************************************/

/* 定义按键值 */
#define KEY0VALUE       1
#define INVAKEY         0

/*
 * @description         : main主程序
 * @param - argc        : argv数组元素个数
 * @param - argv        : 具体参数
 * @return                      : 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
        int fd, led_fd, ret;
        int led_flag = 0;
        char *filename;
        int keyvalue;
        unsigned char  on = 1;
        unsigned char  off= 0;

        if(argc != 2){
                printf("Error Usage!\r\n");
                return -1;
        }

        filename = argv[1];

        /* 打开key驱动 */
        fd = open(filename, O_RDWR);
        //******************************************
        led_fd = open("/dev/dev_led_gpio_14", O_RDWR); 
        //******************************************
        if(fd < 0){
                printf("file %s open failed!\r\n", argv[1]);
                return -1;
        }

        /* 循环读取按键值数据！ */
        while(1) {
                read(fd, &keyvalue, sizeof(keyvalue));
                if (keyvalue == KEY0VALUE) {    /* KEY0 */
                        if(led_flag == 0){
        //******************************************
                            led_flag = 1;
                            write(led_fd, &on, 1);
                        }else if(led_flag ==1){
                            led_flag = 0;
                            write(led_fd, &off, 1);
                        }
        //******************************************
                        printf("KEY0 Press, value = %#X\r\n", keyvalue);        /* 按下 */
                }
        }

        ret= close(fd); /* 关闭文件 */
        if(ret < 0){
                printf("file %s close failed!\r\n", argv[1]);
                return -1;
        }
        return 0;
}
```