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
#include "linuxuart.h"

/*互斥量声明*/
extern pthread_mutex_t 	mutex_client_request,
						mutex_refresh,
						mutex_sqlite,
						mutex_transfer,
						mutex_buzzer,
						mutex_led,
						mutex_fun;
/*条件变量声明*/
extern pthread_cond_t  	cond_client_request,
						cond_refresh,
						cond_sqlite,
						cond_transfer,
						cond_buzzer,
						cond_led,
						cond_fun;
/*消息队列ID*/
extern int msgid;
extern key_t msg_key;
/*消息队列结构体*/
struct msg msgbuf;

extern unsigned char  cmd_led;
extern unsigned char  cmd_buzzer;
extern unsigned char  cmd_seg;
extern unsigned char  cmd_fan;

void *pthread_client_request(void *arg)
{
	/**
	 * 1.申请key
	 * key_t ftok(const char *pathname, int proj_id)
	 */
	if((msg_key = ftok("/tmp", 'g')) < 0){
		perror("ftok err");
		exit(-1);
	}

	/**
	 * 2.打开消息队列
	 * int msgget(key_t key, int msgflg);
	 */
	msgid = msgget(msg_key, IPC_CREAT|IPC_EXCL|0666);
	if(msgid == -1){
		if(errno == EEXIST){
			msgid = msgget(msg_key, 0777);
		}else{
			perror("msgget err");
			exit(1);
		}
	}
	printf("pthread_client_request\n");

	/**
	 * 3.从消息队列接受消息
	 * ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	 */
	while(1){
		bzero(&msgbuf, sizeof(msgbuf));
		printf("wait from client request\n");
		msgrcv(msgid, &msgbuf, sizeof(msgbuf)-sizeof(long), 1, 0);
		printf("Get %ldL msg\n", msgbuf.msgtype);
		printf("text[0] = %#x\n", msgbuf.text[0]);
	
	/**
	 * 4.判断具体的消息类型
	 */
		switch(msgbuf.msgtype){
			case 1L:
				printf("-----------led_pthread----------\n");
				break;
			case 2L:
				printf("-----------buzzer_pthread------------\n");
				break;
			case 3L:
				printf("-----------seg_pthread------------\n");
				break;
			case 4L:
				printf("------------fun_pthread-------------\n");
				pthread_mutex_lock(&mutex_fun);
				cmd_fan = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_fun);
				break;
			case 5L:
				printf("-------------set env data--------------\n");
				break;
			default:
				break;
		}

	}

	exit(0);
}
