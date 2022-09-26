#ifndef __DATA_GLOBAL__H__
#define __DATA_GLOBAL__H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h>



#define MONITOR_NUM   1
#define QUEUE_MSG_LEN  32

#define		GPRS_DEV   			"/dev/ttyUSB0"
#define		ZIGBEE_DEV 		 "/dev/ttyUSB1"
#define		BEEPER_DEV 		 "/dev/fsbeeper0"
#define		LED_DEV    		"/dev/fsled0"


typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;

struct makeru_zigbee_info{
		uint8_t head[3]; 
		uint8_t type;	 //数据类型  'z'---zigbee  'a'---a9
		float temperature; //温度
		float humidity;  //湿度
		float tempMIN;//温度下限
    		float tempMAX;//温度上限 
    		float humidityMIN;   //湿度下限
    		float humidityMAX;   //湿度上限
		uint32_t reserved[2]; //保留扩展位，默认填充0
};

struct makeru_a9_info{
	uint8_t head[3]; 
	uint8_t type;	 
	float adc;
	short gyrox;   
	short gyroy;
	short gyroz;
	short  aacx;  
	short  aacy;
	short aacz;
	uint32_t reserved[2]; 
};

struct makeru_env_data{
	struct makeru_a9_info       a9_info;    
	struct makeru_zigbee_info   zigbee_info;
	uint32_t reserved[2]; 

struct env_info_client_addr
{
	struct makeru_env_data  monitor_no[MONITOR_NUM];	
};

extern void *pthread_client_request (void *arg);
extern void *pthread_refresh(void *arg);           
extern void *pthread_sqlite(void *arg);       
extern void *pthread_transfer(void *arg);  
extern void *pthread_sms(void *arg);      
extern void *pthread_buzzer(void *arg);  
extern void *pthread_led(void *arg); 


extern int send_msg_queue(long type,unsigned char text);

struct msg
{
	long type;//从消息队列接收消息时用于判断的消息类型
	long msgtype;//具体的消息类型
	unsigned char text[QUEUE_MSG_LEN];//消息正文
};




#endif 


