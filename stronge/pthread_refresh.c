/*****************************
 	数据刷新线程
*****************************/

#include "data_global.h"
#include "sem.h"

#define N 1024
extern int shmid;
extern int msgid;
extern int semid;

extern key_t shm_key;
extern key_t sem_key;
extern key_t msg_key;


/*互斥量声明*/
extern pthread_mutex_t mutex_client_request,
				mutex_refresh,
				mutex_sqlite,
				mutex_transfer,
				mutex_buzzer,
				mutex_led,
				mutex_fun;
/*条件变量声明*/
extern pthread_cond_t  cond_client_request,
				cond_refresh,
				cond_sqlite,
				cond_transfer,
				cond_buzzer,
				cond_led,
				cond_fun;

extern struct env_homes homes_env_info;
struct shm_addr
{
	char shm_status;
	struct env_homes all_env_info;
};

struct shm_addr *shm_buf;

int file_env_info_struct(struct env_homes *rt_status, int home_id);


void *pthread_refresh(void *arg)
{	//申请key
	if((sem_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	//创建信号灯
	semid = semget(sem_key,1,IPC_CREAT|IPC_EXCL|0666);
	if(semid == -1)	{
		if(errno == EEXIST){
			semid = semget(sem_key,1,0777);
		}else{
			perror("fail to semget");
			exit(1);
		}
	}else{
		init_sem (semid, 0, 1);
	}

	//share memory for env_info refresh config
	if((shm_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	shmid = shmget(shm_key,N,IPC_CREAT|IPC_EXCL|0666);
	if(shmid == -1)	{
		if(errno == EEXIST){
			shmid = shmget(msg_key,N,0777);
		}else{
			perror("fail to shmget");
			exit(1);
		}
	}

	//share memap
	if((shm_buf = (struct shm_addr *)shmat(shmid,NULL,0)) == (void *)-1)
	{
		perror("fail to shmat");
		exit(1);
	}

	printf("pthread_refresh ......>>>>>>>\n");
	bzero (shm_buf, sizeof (struct shm_addr));
	while(1){
		//p操作
		sem_p(semid,0);
		shm_buf->shm_status = 1;
		int home_id = 1;
#if 1 	
		//真实数据上传
		shm_buf->all_env_info.home[home_id] = homes_env_info.home[home_id];
#else
		file_env_info_struct(&shm_buf->homes_env_info,shm_buf->shm_status);
#endif 
		sleep(1);
		sem_v(semid,0);
		pthread_cond_signal(&cond_transfer);
	}
	printf("pthread_refresh\n");
	exit(0);
}


int file_env_info_struct(struct env_homes *rt_status, int home_id)
{
	int  env_info_size = sizeof(struct env_homes);
	//	printf("env_info_size = %d.\n",env_info_size);

	rt_status->home[home_id].data_zigbee.temperature = 10.0;
	rt_status->home[home_id].data_zigbee.tempMIN = 2.0;
	rt_status->home[home_id].data_zigbee.tempMAX = 20.0;
	rt_status->home[home_id].data_zigbee.humidity  = 20.0;
	rt_status->home[home_id].data_zigbee.humidityMIN  = 10.0;
	rt_status->home[home_id].data_zigbee.humidityMAX  = 30.0;
	rt_status->home[home_id].data_zigbee.reserved[0]  = 0.01;
	rt_status->home[home_id].data_zigbee.reserved[1]  = -0.01;


	rt_status->home[home_id].data_a9.adc  = 9.0;
	rt_status->home[home_id].data_a9.gyrox  = -14.0;
	rt_status->home[home_id].data_a9.gyroy  = 20.0;
	rt_status->home[home_id].data_a9.gyroz  = 40.0;
	rt_status->home[home_id].data_a9.aacx  = 642.0;
	rt_status->home[home_id].data_a9.aacy  = -34.0;
	rt_status->home[home_id].data_a9.aacz  = 5002.0;
	rt_status->home[home_id].data_a9.reserved[0]  = 0.01;
	rt_status->home[home_id].data_a9.reserved[1]  = -0.01;

	return 0;
}
