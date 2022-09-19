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

void *pthread_client_request(void *arg)
{
	/**
	 * 1.申请key
	 * key_t ftok(const char *pathname, int proj_id)
	 */

	/**
	 * 2.打开消息队列
	 * int msgget(key_t key, int msgflg);
	 */

	/**
	 * 3.从消息队列接受消息
	 * ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	 */
	
	/**
	 * 4.判断具体的消息类型
	 */
	
	exit(0);
}
