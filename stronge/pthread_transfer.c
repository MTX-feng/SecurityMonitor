/****************************
	数据采集线程
*****************************/

#include "data_global.h"
#include "mpu6050.h"
#include <strings.h>

//接收ZigBee的数据和采集的A9平台的传感器数据
int adc_fd;
int mpu_fd;
int zigbee_fd;

extern pthread_cond_t cond_transfer;
extern pthread_mutex_t mutex_transfer;
extern struct env_homes homes_env_info;

int env_info_a9_zigbee(struct env_homes *data, int home_id);
//int printf_sensor_info_debug(struct env_homes *data, int home_id);

void *pthread_transfer(void *arg)
{
	printf("pthread_transfer\n");
	int home_id = 1;
	/*打开设备*/
	adc_fd = open(ADC_DEV, O_RDWR);
	mpu_fd = open(MPU6050_DEV, O_RDWR);
	if((adc_fd == -1) || (mpu_fd == -1)){
		perror("open adc or mpu6050 failed");
	}
	while(1){
		pthread_mutex_lock(&mutex_transfer);
		pthread_cond_wait(&cond_transfer, &mutex_transfer);
		printf("pthread_transfer lock\n");

		/*采集数据*/
		env_info_a9_zigbee(&homes_env_info, home_id);
		sleep(1);
	}
	close(adc_fd);
	close(mpu_fd);

	exit(0);
}

int env_info_a9_zigbee(struct env_homes *data, int home_id){
	int env_info_size = sizeof(struct env_homes);
	printf("env_info_size = %d\n", env_info_size);

	data->home[home_id].data_zigbee.head[0] = 'm';
	data->home[home_id].data_zigbee.head[1] = 's';
	data->home[home_id].data_zigbee.head[2] = 'm';
	data->home[home_id].data_zigbee.type = 's';
	data->home[home_id].data_zigbee.temperature = 10;
	data->home[home_id].data_zigbee.tempMAX = 20;
	data->home[home_id].data_zigbee.tempMIN = 10;
	data->home[home_id].data_zigbee.humidity = 20;
	data->home[home_id].data_zigbee.humidityMAX = 30;
	data->home[home_id].data_zigbee.humidityMIN = 20;
	data->home[home_id].data_zigbee.reserved[0] = 0.01;
	data->home[home_id].data_zigbee.reserved[1] = -0.01;

	int adc_data;
	struct mpu6050_data mpu_data;

	/*获取adc数据*/
	read(adc_fd, &adc_data, 4);
	printf("adc_value = %0.2fV\n", (1.8*adc_data)/4096);
	data->home[home_id].data_a9.adc = (float)((1.8*adc_data)/4096);
	/*获取mpu6050数据*/
	ioctl(mpu_fd,MPU6050_GYRO,&mpu_data);
	printf("gyro data: x = %d, y = %d, z = %d\n", mpu_data.gyro.x,mpu_data.gyro.y,mpu_data.gyro.z);
	ioctl(mpu_fd,MPU6050_ACCEL,&mpu_data);
	printf("accel data: x = %d, y = %d, z = %d\n", mpu_data.accel.x,mpu_data.accel.y,mpu_data.accel.z);
	
	data->home[home_id].data_a9.head[0]  = 'm';
	data->home[home_id].data_a9.head[1]  = 's';
	data->home[home_id].data_a9.head[2]  = 'm';
	data->home[home_id].data_a9.type  = 'a';

	data->home[home_id].data_a9.gyrox  =  (short)mpu_data.gyro.x;
	data->home[home_id].data_a9.gyroy  =  (short)mpu_data.gyro.y;
	data->home[home_id].data_a9.gyroz  =  (short)mpu_data.gyro.z;

	data->home[home_id].data_a9.aacx   =  (short)mpu_data.accel.x;
	data->home[home_id].data_a9.aacy   =  (short)mpu_data.accel.y;
	data->home[home_id].data_a9.aacz   =  (short)mpu_data.accel.z;
	data->home[home_id].data_a9.reserved[0]  = 0.01;
	data->home[home_id].data_a9.reserved[1]  = -0.01;

	//printf_sensor_info_debug(mpu_data,home_id);

	return 0;
}


int printf_sensor_info_debug(struct env_homes *data, int home_id)
{
	printf("a9_info.adc  : %f.\n",data->home[home_id].data_a9.adc);
	printf("a9_info.gyrox: %d.\n",data->home[home_id].data_a9.gyrox);
	printf("a9_info.gyroy: %d.\n",data->home[home_id].data_a9.gyroy);
	printf("a9_info.gyroz: %d.\n",data->home[home_id].data_a9.gyroz);
	printf("a9_info.aacx : %d.\n",data->home[home_id].data_a9.aacx );
	printf("a9_info.aacy : %d.\n",data->home[home_id].data_a9.aacy);
	printf("a9_info.aacz : %d.\n",data->home[home_id].data_a9.aacz);

	return 0;
}