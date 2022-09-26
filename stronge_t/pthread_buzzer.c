#include "data_global.h"
//#include "buzzer.h"

#define	MAGIC_NUM		'k'
#define BEEP_ON			_IO(MAGIC_NUM, 0)
#define BEEP_OFF		_IO(MAGIC_NUM, 1)
#define BEEP_FREQ		_IO(MAGIC_NUM, 2)

extern pthread_mutex_t mutex_debug;
extern pthread_cond_t cond_debug;

void *pthread_buzzer(void *arg)
{
	printf("pthread_buzzer\n");

#if 1
	int i = 0;

	int buzzer_fd;

	//buzzer_fd = open(BEEPER_DEV,O_RDWR | O_NONBLOCK);

	buzzer_fd = open(BEEPER_DEV, O_RDWR);
	//buzzer_fd = open("/dev/test", O_RDWR|O_CREAT);
	if ( buzzer_fd == -1 ) {
		perror("open buzzer failed.\n");
		//return -1;
	}
	printf("buzzer_fd :%d.\n",buzzer_fd);	

	while(1){
		pthread_mutex_lock(&mutex_debug);
		printf("%s %d %s", __FILE__,__LINE__,__func__);
		pthread_cond_wait(&cond_debug, &mutex_debug);
		printf("%s %d %s", __FILE__,__LINE__,__func__);
		if(cmd_buzzer == 0x51)
		{
			printf("buzzer_fd ON\n");
			ioctl(buzzer_fd,BEEP_ON);	
		}
		else if(cmd_buzzer == 0x50)
		{
			printf("buzzer_fd OFF\n");
			ioctl(buzzer_fd,BEEP_OFF);
		}
		else 
		{
			printf("cmd_buzzer error.\n");
		}
		pthread_mutex_unlock(&mutex_debug);
	}
	close(buzzer_fd);
	//exit(0);
#endif

}



