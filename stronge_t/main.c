#include "data_global.h"

void release_pthread_resource(int signo);
/*互斥量声明*/
extern pthread_mutex_t mutex_client_request,
        		mutex_refresh,
        		mutex_sqlite,
	        	mutex_transfer,
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
	        	cond_transfer,
	        	cond_fan,
	        	cond_zigbee,
	        	cond_buzzer,
	        	cond_debug,
	         	cond_led;

/*消息队列ID*/
extern int msgid;
/*共享内存ID*/
extern int shmid;
/*信号灯集ID*/
extern int semid;

pthread_t  id_client_request,
		   id_refresh,
		   id_sqlite,
		   id_transfer,
		   id_fan,
		   id_buzzer,
		   id_led;		

int main(int argc, const char *argv[])
{
	/*互斥锁初始化*/
	pthread_mutex_init(&mutex_client_request,NULL);
	pthread_mutex_init(&mutex_refresh,NULL);
	pthread_mutex_init(&mutex_sqlite,NULL);
	pthread_mutex_init(&mutex_transfer,NULL);
	pthread_mutex_init(&mutex_fan,NULL);
	pthread_mutex_init(&mutex_zigbee,NULL);
	pthread_mutex_init(&mutex_buzzer,NULL);
	pthread_mutex_init(&mutex_debug,NULL);
	pthread_mutex_init(&mutex_led,NULL);
	
	//等待接受信号，信号处理函数
	/*收到终止信号，释放线程资源
		SIGINT----CTRL C
		release_pthread_resource----释放线程资源函数
	*/
	signal (SIGINT, release_pthread_resource);

	/*初始化条件变量*/
	pthread_cond_init(&cond_client_request,NULL);
	pthread_cond_init(&cond_refresh,NULL);
	pthread_cond_init(&cond_sqlite,NULL);
	pthread_cond_init(&cond_transfer,NULL);
	pthread_cond_init(&cond_fan,NULL);
	pthread_cond_init(&cond_zigbee,NULL);
	pthread_cond_init(&cond_buzzer,NULL);
	pthread_cond_init(&cond_debug,NULL);
	pthread_cond_init(&cond_led,NULL);

	/*创建线程*/
	pthread_create(&id_client_request, NULL,pthread_client_request,NULL);   
	pthread_create(&id_refresh, NULL,pthread_refresh,NULL);  			
	pthread_create(&id_sqlite, NULL,pthread_sqlite,NULL);  			
	pthread_create(&id_transfer, NULL,pthread_transfer,NULL); 
	pthread_create(&id_fan,	NULL,pthread_fan,NULL);  	
	pthread_create(&id_buzzer, NULL,pthread_buzzer,NULL);	 
	pthread_create(&id_led,	NULL,pthread_led,NULL);  	

	/*等待线程结束回收线程*/
	pthread_join(id_client_request,NULL);   printf ("pthread1\n");
	pthread_join(id_refresh,NULL);          printf ("pthread2\n");
	pthread_join(id_sqlite,NULL);			printf ("pthread3\n");
	pthread_join(id_transfer,NULL);			printf ("pthread4\n");
	pthread_join(id_fan,NULL);				printf ("pthread5\n");
	pthread_join(id_buzzer,NULL);			printf ("pthread6\n");
	pthread_join(id_led,NULL);				printf ("pthread7\n");

	return 0;
}


void release_pthread_resource(int signo)
{
	/*释放锁*/
	pthread_mutex_destroy (&mutex_client_request);   
	pthread_mutex_destroy (&mutex_refresh);   
	pthread_mutex_destroy (&mutex_sqlite);    
	pthread_mutex_destroy (&mutex_transfer);   
	pthread_mutex_destroy (&mutex_fan); 
	pthread_mutex_destroy (&mutex_zigbee); 
	pthread_mutex_destroy (&mutex_buzzer); 
	pthread_mutex_destroy (&mutex_debug);  
	pthread_mutex_destroy (&mutex_led);   

	/*释放条件变量*/
 	pthread_cond_destroy (&cond_client_request);
 	pthread_cond_destroy (&cond_refresh);
 	pthread_cond_destroy (&cond_sqlite);
 	pthread_cond_destroy (&cond_transfer);
 	pthread_cond_destroy (&cond_fan);
 	pthread_cond_destroy (&cond_zigbee);
 	pthread_cond_destroy (&cond_buzzer);
 	pthread_cond_destroy (&cond_debug);
 	pthread_cond_destroy (&cond_led);

	/*回收线程———线程分离*/
	pthread_detach(id_client_request);
	pthread_detach(id_refresh);
	pthread_detach(id_sqlite);
	pthread_detach(id_transfer);
	pthread_detach(id_fan);
	pthread_detach(id_buzzer);
	pthread_detach(id_led);

	printf("all pthread is detached\n");
	/*删除消息队列*/
	msgctl(msgid, IPC_RMID, NULL);
	/*删除共享内存*/
	shmctl(shmid, IPC_RMID, NULL);
	/*删除信号灯*/
	semctl(semid, 1, IPC_RMID, NULL);	 

	exit(0);
}



