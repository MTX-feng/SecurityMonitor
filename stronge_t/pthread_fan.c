#include "data_global.h"
#include "linuxuart.h"

extern pthread_cond_t cond_zigbee;
extern pthread_mutex_t mutex_zigbee;

void *pthread_fan(void *arg)
{

	printf("pthread_fan\n");

	int fan_fd = open_port(ZIGBEE_DEV);
	if(fan_fd < 0){
		printf("open failed\n");
	}
	set_com_config(fan_fd, 115200, 8, 'N', 1);

	while(1){
		printf("%s %d %s\n", __FILE__,__LINE__,__func__);
		pthread_mutex_lock(&mutex_zigbee);
		printf("%s %d %s\n", __FILE__,__LINE__,__func__);
		pthread_cond_wait(&cond_zigbee, &mutex_zigbee);
		char cmdbuf[4] = {0};
		if(cmd_fan > 0x21){
			printf("----------fan_ON------------\n");
			strcpy(cmdbuf,"1\n");
			write(fan_fd,cmdbuf,sizeof(cmdbuf)/sizeof(cmdbuf[0]));	
			sleep(2);
		}
		if(cmd_fan == 0x20){
			printf("----------fan_OFF------------\n");
			strcpy(cmdbuf,"0\n");
			write(fan_fd,cmdbuf,sizeof(cmdbuf)/sizeof(cmdbuf[0]));
			sleep(2);
		}
		char buf[32] = {0};
			//get data from zigbee
			read(fan_fd,&buf,sizeof(buf));
			if(strlen(buf) > 0){
				printf("sizeof(buf) = %ld.\n",sizeof(buf));
				printf(">>>>>>%s\n",buf);	
			}
			sleep(1);
		pthread_mutex_unlock(&mutex_zigbee);
	}

	
	close(fan_fd);
}



