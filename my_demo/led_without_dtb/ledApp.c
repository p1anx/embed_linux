#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  int  fd;
  int cmd;
  int data;
  fd = open("/dev/pin4", O_RDWR);
  if(fd < 0)
  {
    printf("open failed\n");
    perror("reson: \n");
  }
  else {
    printf("open success\n");
  }
  printf("input command: 1/0 \n1:set pin4 high \n0:set pin4 low\n");
  scanf("%d", &cmd);
  if(cmd ==1){
    data = 1;
  }
  if(cmd ==0){
    data = 0;
  }
  fd = write(fd, &data, 1);
  return 0;
}
