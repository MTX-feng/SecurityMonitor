/********************************************
  	全局的互斥体声明
    全局的条件锁声明声明
    全局的id和key信息声明
    全局的消息队列发送函数声明
 ********************************************/
#include "data_global.h"
/*互斥量声明*/
pthread_mutex_t mutex_client_request,
        		mutex_refresh,
        		mutex_sqlite,
	        	mutex_transfer,
	        	mutex_fan,
	        	mutex_zigbee,
	        	mutex_buzzer,
	        	mutex_debug,
	        	mutex_camera,
	         	mutex_led;

/*条件变量声明*/
pthread_cond_t  cond_client_request,
        		cond_refresh,
        		cond_sqlite,
	        	cond_transfer,
	        	cond_fan,
	        	cond_zigbee,
	        	cond_buzzer,
				cond_led,
				cond_debug;
/*消息队列ID*/
int msgid;
/*共享内存ID*/
int shmid;
/*信号灯集ID*/
int semid;

key_t key; //msg_key
key_t shm_key;
key_t sem_key;

/*所有房间环境信息*/
struct env_info_client_addr  sm_all_env_info;
//发送消息队列
int send_msg_queue(long type,unsigned char text)
{
	struct msg msgbuf;
	msgbuf.type = 1L;
	msgbuf.msgtype = type;
	msgbuf.text[0] = text;

	if(msgsnd(msgid,&msgbuf,sizeof(msgbuf) - sizeof(long),0) == -1){
		perror("fail to msgsnd type2");
		exit(1);
	}

	return 0;
}








