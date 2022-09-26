#ifndef __DATA_GLOBAL__H__
#define __DATA_GLOBAL__H__

/*
  全局的宏定义#define
  全局的线程函数声明
  全局的设备节点声明
  全局的消息队列发送函数外部extern声明
  全局的消息队列传递的结构体信息声明
 */

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

/*********************************************************
  全局的宏定义
 *********************************************************/
#define 	HOME_NUM       1
#define 	QUEUE_MSG_LEN  32

/********************************************************
  对所使用设备的定义
 ********************************************************/
#define		ZIGBEE_DEV 		 "/dev/ttyUSB1"     //zigbee
#define		BEEPER_DEV 		 "/dev/fsbeeper0"   //蜂鸣器
#define		LED_DEV    		 "/dev/fsled0" 			//led灯
#define		ADC_DEV   	   "/dev/fsadc0"  		//ADC
#define   MPU6050_DEV    "/dev/fsmpu60500"  //mpu6050


unsigned char  cmd_led;
unsigned char  cmd_buzzer;
unsigned char  cmd_seg;
unsigned char  cmd_fan;

/*********************************************************
  全局的结构体声明
 *********************************************************/
typedef  unsigned char 		uint8_t;
typedef  unsigned short 	uint16_t;
typedef  unsigned int 		uint32_t;

//考虑到内存对齐的问题
/*zigbee所采集的数据*/
struct zigbee_data{
	uint8_t 	head[3]; 			//标识位: 'm' 's' 'm'  makeru-security-monitor  
	uint8_t 	type;	 				//数据类型  'z'---zigbee  'a'---a9
	float 		temperature;  //温度
	float 		humidity;     //湿度
	float 		tempMIN;  		//温度下限
	float   	tempMAX;			//温度上限 
	float 		humidityMIN;  //湿度下限
	float 		humidityMAX;  //湿度上限
	uint32_t  reserved[2]; 	//保留扩展位，默认填充0
};

/*a9采集的数据*/
struct a9_data{
	uint8_t 	head[3]; 			//标识位: 'm' 's' 'm'  makeru-security-monitor  
	uint8_t 	type;	 				//数据类型  'z'---zigbee  'a'---a9
	float 		adc;					//电压数据
	short 		gyrox;   			//陀螺仪数据
	short 		gyroy;
	short 		gyroz;
	short  		aacx;  				//加速计数据
	short  		aacy;
	short 		aacz;
	uint32_t  reserved[2];  //保留扩展位，默认填充0
};

/*环境信息*/
struct env_info{
	struct a9_data				data_a9;  		//a9环境信息    
	struct zigbee_data		data_zigbee;  //zigbee环境信息
	uint32_t reserved[2]; 							//保留扩展位，默认填充0
};

//所有房间的信息结构体
struct env_homes
{
	struct env_info  home[HOME_NUM];	//数组 home1  home2 
};

/*********************************************************
  全局的外部线程函数声明
 *********************************************************/

extern void *pthread_client_request(void *arg); 			//接收用户请求线程
extern void *pthread_refresh(void *arg);              //数据刷新线程
extern void *pthread_sqlite(void *arg);               //数据库线程，保存数据库的数据
extern void *pthread_transfer(void *arg);             //数据采集线程
extern void *pthread_buzzer(void *arg);               //蜂鸣器控制线程
extern void *pthread_led(void *arg);                  //led灯控制线程
extern void *pthread_seg(void *arg);                  //led灯模拟数码管控制线程
extern void *pthread_fun(void *arg);                  //zigbee风扇控制线程
//extern void *pthread_sms(void *arg);                  //GPRS线程

extern int send_msg_queue(long type,unsigned char text); //发送消息队列

/*********************************************************
  全局的消息队列传递的结构体声明
 *********************************************************/
//消息队列结构体
struct msg
{
	long type;												//从消息队列接收消息时用于判断的消息类型
	long msgtype;											//具体的消息类型
	unsigned char text[QUEUE_MSG_LEN];//消息正文
};

#endif 

