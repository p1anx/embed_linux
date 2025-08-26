#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/***************************************************************
文件名		: ledapp.c
作者	 	  : xwj
版本	  	: V1.0
描述	  	: chrdevbase驱测试APP。
其他	  	: 无
使用方法	：./ledapp /dev/dev_led  0 关闭LED
            ./ledapp /dev/dev_led  1 打开LED
日志	  	: 初版V1.0 2025/8/26 xwj
***************************************************************/
int main(int argc, char *argv[]) {
  printf("led test_app\n");

  /*判断输入的命令是否合法*/
  if (argc != 3) {
    printf(" command error ! \n");
    printf(" usage : ./ledapp /dev/dev_led 0 [num can be 0 or 1]\n");
    return -1;
  }

  /*打开文件*/
  char *filename = argv[1];
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    printf("open file : %s failed !\n", argv[0]);
    return -1;
  }

  unsigned char commend = atoi(argv[2]); // 将接收值转化为数字;

  /*写入命令*/
  int error = write(fd, &commend, sizeof(commend));
  if (error < 0) {
    printf("write file error! \n");
    close(fd);
    /*判断是否关闭成功*/
  }

  /*关闭文件*/
  error = close(fd);
  if (error < 0) {
    printf("close file error! \n");
  }

  return 0;
}
