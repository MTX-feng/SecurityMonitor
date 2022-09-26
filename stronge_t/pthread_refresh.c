/*****************************
 	数据刷新线程
*****************************/
#include "data_global.h"
#include "sem.h"

#define N 1024  //for share memory
extern int shmid;
extern int msgid;
extern int semid;

extern key_t shm_key;
extern key_t sem_key;
extern key_t key; //msg_key

extern pthread_mutex_t mutex_client_request,
	   mutex_refresh,
	   mutex_sqlite,
	   mutex_transfer,
	   mutex_analysis,
	   mutex_fan,
	   mutex_buzzer,
	   mutex_led,
	   mutex_camera;

extern pthread_cond_t  cond_client_request,
	   cond_refresh,
	   cond_sqlite,
	   cond_transfer,
	   cond_analysis,
	   cond_fan,
	   cond_buzzer,
	   cond_led,
	   cond_camera;


extern struct env_info_client_addr  sm_all_env_info;

struct shm_addr
{
	char shm_status;
	struct env_info_client_addr  sm_all_env_info;
};
struct shm_addr *shm_buf;

int file_env_info_struct(struct env_info_client_addr  *rt_status,int home_id);

void *pthread_refresh(void *arg)
{
	//申请key
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
			shmid = shmget(key,N,0777);
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
		shm_buf->sm_all_env_info.monitor_no[home_id] = sm_all_env_info.monitor_no[home_id];
#else
		//模拟数据
		file_env_info_struct(&shm_buf->sm_all_env_info,shm_buf->shm_status); 
#endif 
		sleep(1);
		//v操作
		sem_v(semid,0);
		pthread_cond_signal(&cond_transfer);
	}


}



int file_env_info_struct(struct env_info_client_addr *rt_status,int home_id)
{
	int  env_info_size = sizeof(struct env_info_client_addr);
	//	printf("env_info_size = %d.\n",env_info_size);

	rt_status->monitor_no[home_id].zigbee_info.temperature = 10.0;
	rt_status->monitor_no[home_id].zigbee_info.tempMIN = 2.0;
	rt_status->monitor_no[home_id].zigbee_info.tempMAX = 20.0;
	rt_status->monitor_no[home_id].zigbee_info.humidity  = 20.0;
	rt_status->monitor_no[home_id].zigbee_info.humidityMIN  = 10.0;
	rt_status->monitor_no[home_id].zigbee_info.humidityMAX  = 30.0;
	rt_status->monitor_no[home_id].zigbee_info.reserved[0]  = 0.01;
	rt_status->monitor_no[home_id].zigbee_info.reserved[1]  = -0.01;


	rt_status->monitor_no[home_id].a9_info.adc  = 9.0;
	rt_status->monitor_no[home_id].a9_info.gyrox  = -14.0;
	rt_status->monitor_no[home_id].a9_info.gyroy  = 20.0;
	rt_status->monitor_no[home_id].a9_info.gyroz  = 40.0;
	rt_status->monitor_no[home_id].a9_info.aacx  = 642.0;
	rt_status->monitor_no[home_id].a9_info.aacy  = -34.0;
	rt_status->monitor_no[home_id].a9_info.aacz  = 5002.0;
	rt_status->monitor_no[home_id].a9_info.reserved[0]  = 0.01;
	rt_status->monitor_no[home_id].a9_info.reserved[1]  = -0.01;

	return 0;
}







