/*********************************
	接受用户请求线程
	对接收到不同的消息类型进行判断处理
		消息类型分配
		1L：	LED控制 			pthread_led
		2L：蜂鸣器控制 		pthread_buzzer
		3L：模拟数码管       	pthread_seg
		4L：zigbee风扇		pthread_fun
		5L：
		6L：
		...
 *********************************/
#include "data_global.h"

#if 0
#define	MAGIC_NUM		'k'
#define BEEP_ON			_IO(MAGIC_NUM, 0)
#define BEEP_OFF		_IO(MAGIC_NUM, 1)
#define BEEP_FREQ		_IO(MAGIC_NUM, 2)

#endif

/*互斥量声明*/
extern pthread_mutex_t mutex_client_request,
	   mutex_refresh,
	   mutex_sqlite,
	   mutex_transfer,
	   mutex_analysis,
	   mutex_fan,
	   mutex_zigbee,
	   mutex_buzzer,
	   mutex_debug,
	   mutex_led;
/*条件变量声明*/
extern pthread_cond_t  cond_client_request,
	   cond_refresh,
	   cond_sqlite,
	   cond_transfer,
	   cond_analysis,
	   cond_fan,
	   cond_zigbee,
	   cond_buzzer,
	   cond_debug,
	   cond_led;

extern unsigned char cmd_led;
extern unsigned char cmd_buzzer;
extern unsigned char cmd_seg;
extern unsigned char cmd_fan;
/*消息队列结构体*/
struct msg msgbuf;
/*消息队列ID*/
extern int msgid;
extern key_t key;


void *pthread_client_request(void *arg)
{
	/**
	 * 1.申请key
	 * key_t ftok(const char *pathname, int proj_id)
	 */
	if((key = ftok("/tmp",'g')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	/**
	 * 2.打开消息队列
	 * int msgget(key_t key, int msgflg);
	 */	
	msgid = msgget(key,IPC_CREAT|IPC_EXCL|0666);
	if(msgid == -1)	{
		if(errno == EEXIST){
			msgid = msgget(key,0777);
		}else{
			perror("fail to msgget");
			exit(1);
		}
	}
	printf("pthread_client_request\n");

	/**
	 * 3.从消息队列接受消息
	 * ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	 */
	while(1){
		bzero(&msgbuf,sizeof(msgbuf));
		printf("wait form client request...\n");
		msgrcv (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 1L, 0);
		printf ("Get %ldL msg\n", msgbuf.msgtype);
		printf ("text[0] = %#x\n", msgbuf.text[0]);
	
		/**
	 	 * 4.判断具体的消息类型
	 	 */
		switch(msgbuf.msgtype){
			case 1L: 	
				pthread_mutex_lock(&mutex_led);
				printf("%s---%d----%s\n", __FILE__,__LINE__,__func__);    
				printf("hello led\n");
				cmd_led = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_led);
				pthread_cond_signal(&cond_led);
				break;
			case 2L:
				printf("%s---%d----%s\n", __FILE__,__LINE__,__func__);    
				//pthread_mutex_lock(&mutex_buzzer);
				pthread_mutex_lock(&mutex_debug);
				printf("hello beep\n");
				cmd_buzzer = msgbuf.text[0];
#if 0
				int i = 0;

				int buzzer_fd;

	//buzzer_fd = open(BEEPER_DEV,O_RDWR | O_NONBLOCK);

	//buzzer_fd = open(BEEPER_DEV, O_RDWR);
	buzzer_fd = open("/dev/test", O_RDWR|O_CREAT);
	if ( buzzer_fd == -1 ) {
		perror("open buzzer failed.\n");
		//return -1;
	}
	printf("buzzer_fd :%d.\n",buzzer_fd);	

	while(1){
		printf("%s %d %s", __FILE__,__LINE__,__func__);
		if(cmd_buzzer == 0x51)
		{
			printf("buzzer_fd ON\n");
			//ioctl(buzzer_fd,BEEP_ON);	
		}
		else if(cmd_buzzer == 0x50)
		{
			printf("buzzer_fd OFF\n");
			//ioctl(buzzer_fd,BEEP_OFF);
		}
		else 
		{
			printf("cmd_buzzer error.\n");
		}
		pthread_mutex_unlock(&mutex_buzzer);
	}
	close(buzzer_fd);
#endif
				//pthread_mutex_unlock(&mutex_buzzer);
				//pthread_cond_signal(&cond_sqlite);
				pthread_mutex_unlock(&mutex_debug);
				pthread_cond_signal(&cond_debug);
				break;
			case 3L:
				pthread_mutex_lock(&mutex_led);
				printf("%s----%d----%s\n", __FILE__,__LINE__,__func__);
				printf("hello seg\n");
				cmd_seg = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_led);
				pthread_cond_signal(&cond_led);
				break;
			case 4L:
				printf("%s----%d----%s\n", __FILE__,__LINE__,__func__);
				pthread_mutex_lock(&mutex_zigbee);
				printf("%s----%d----%s\n", __FILE__,__LINE__,__func__);
				printf("hello fan\n");
				cmd_fan = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_zigbee);
				pthread_cond_signal(&cond_zigbee);
				break;
			case 5L:
				printf("set env data\n");
				printf("temMAX: %d\n",*((int *)&msgbuf.text[1]));
				printf("temMIN: %d\n",*((int *)&msgbuf.text[5]));
				printf("humMAX: %d\n",*((int *)&msgbuf.text[9]));
				printf("humMAX: %d\n",*((int *)&msgbuf.text[13]));
				printf("illMAX: %d\n",*((int *)&msgbuf.text[17]));
				printf("illMAX: %d\n",*((int *)&msgbuf.text[21]));

				break;
			case 6L:
			case 7L:
			case 8L:
			case 9L:
				break;
			default:
				break;

		}
	}

}
