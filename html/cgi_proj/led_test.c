#include <stdio.h> 
#include "cgic.h" 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ioctl.h>

#define mytype 'g'

typedef struct led_desc{
	int led_num;    //2 3 4 5
	int led_state;  //0 or 1
}led_desc_t;

#define FS_LED_ON  _IO(mytype,1)
#define FS_LED_OFF _IO(mytype,0)

#define LED "/dev/led_test"


int main(int argc, char const *argv[])
{
	int i = 0,j = 3;
	int nread;
	int led_control,led_state;
	int led_fd,fifo_fd;
	led_desc_t led;
	char *data;

	led_fd = open(LED,O_RDWR);
	if(led_fd < 0){
		printf("open failed !\n");
	}
	printf("open device success! led_fd: %d\n",led_fd);

    printf("Content-type: text/html;charset=utf-8\n\n");
    printf("<html>\n");
    printf("<head><title>web 点灯</title></head>\n");
    printf("<body>\n");
    printf("<p>led is setted successful! you can watch the led's change</p>\n");
    //printf("<p><a herf=http://192.168.1.100/led.html>go back</a></p>\n");
	printf("<a href=\"/led_test.html\">go back led control page </a>");
	printf("</body>\n");

    data = getenv("QUERY_STRING");   //getenv()读取环境变量的当前值的函数 

    if(sscanf(data,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
    {   //利用sscnaf（）函数的特点将环境变量分别提取出led_control和led_state这两个值
        printf("<p>please input right"); 
        printf("</p>");
    } 
    printf("<p>led_control = %d,led_state =  %d</p>", led_control, led_state);
    if(led_control < 2 || led_control > 5) { 
        printf("<p>Please input 2<=led_control<=5!"); 
        printf("</p>");
    } 
    if(led_state>1) {
        printf("<p>Please input 0<=led_state<=1!"); 
        printf("</p>"); 
    }

	led.led_num   = led_control;
	led.led_state = led_state;
	
	if(led.led_state == 0){
		//关灯
		ioctl(led_fd,FS_LED_OFF, led.led_num);
	}else if(led.led_state == 1){
		ioctl(led_fd,FS_LED_ON, led.led_num);
	}else if(led.led_state == 2){
		while(j --){
			for(i = 2; i <= 5; i ++ ){
				led.led_num = i;
				led.led_state = 0;
				ioctl(led_fd,FS_LED_OFF, led.led_num);
				usleep(500000);
				led.led_state = 1;
				ioctl(led_fd,FS_LED_ON, led.led_num);
				usleep(500000);
			}
		}
	}

	close(led_fd);
    printf("</html>\n");

	return 0;
}