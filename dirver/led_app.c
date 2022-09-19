/*************************************************************************
    > File Name: led_app.c
    > Author: wrf
    > Mail: wrf6758@qq.com 
    > Created Time: 2022年09月13日 星期二 16时27分37秒
    > led 测试app 
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "led_dev.h"

int main(int argc, char *argv[])
{
    int fd = -1;
    int on_off = 0;
    int nu = 0;

    if(argc < 4)
    {
        printf("the argument is too few\n");
        return -1;
    }

    sscanf(argv[2], "%d", &on_off);
    sscanf(argv[3], "%d", &nu);
    
    fd = open(argv[1], O_RDONLY);
    if(fd < 0)
    {
        printf("open %s failed\n", argv[1]);
        return 1;
    }   
    
    if(on_off)
    {
        ioctl(fd, MY_LED_ON, nu);
    }
    else
    {
        ioctl(fd, MY_LED_OFF, nu);
    }
    close(fd);
    fd = -1;

    return 0;
}
