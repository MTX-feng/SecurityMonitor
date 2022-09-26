/************************
	zigbee风扇控制线程
************************/

#include "linuxuart.h"

void *pthread_fun(void *arg)
{ 
	printf("pthread_fun\n");
	int fan_fd = open_port("/dev/ttyUSB0"); //打开设备
	if(fan_fd < 0){
		printf("open failed\n");
	}
	set_com_config(fan_fd, 115200, 8, 'N', 1); //设置串口参数
				
	char cmdbuf[4] = {0};
	if(cmd_fan == 0x21){
		strcpy(cmdbuf,"1\n"); //注意点
		write(fan_fd,cmdbuf,sizeof(cmdbuf)/sizeof(cmdbuf[0]));
		sleep(2);
	}
	if(cmd_fan == 0x20){
		strcpy(cmdbuf,"0\n");
		write(fan_fd,cmdbuf,sizeof(cmdbuf)/sizeof(cmdbuf[0]));
		sleep(2);
	}
	char buf[32] = {0};
	//get data from zigbee
	read(fan_fd,&buf,sizeof(buf));
	printf("sizeof(buf) = %d.\n",sizeof(buf));
	printf(">>>>>>%s\n",buf);
	sleep(1);

	exit(0);
}
