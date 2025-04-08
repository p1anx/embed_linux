#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
        int fd;
        unsigned char on = 1;
        unsigned char off = 0;
        fd = open("/dev/led3_cdev", O_RDWR);
        if(fd < 0){
                printf("failed to open\n");
                return -1;
        }
        printf("success to open\n");
        while(1){
                write(fd, &on, 1); 
                printf("is blinking\n");
                sleep(1);
                write(fd, &off, 1); 
                sleep(1);
        }

        return 0;
}