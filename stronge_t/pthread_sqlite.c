
#include "data_global.h"
void *pthread_sqlite(void *arg)
{
	printf("pthread_sqlite\n");
}

#if 0
#include <sqlite3.h>

extern pthread_mutex_t mutex_sqlite;
extern pthread_cond_t cond_sqlite;

extern struct env_info_client_addr  sm_all_env_info;

#define DATABASE "my_info.db"

char *env_info_struct(struct env_info_client_addr  *rt_status,int home_id);
int get_date(char *date);
int do_query(sqlite3 *db);

void *pthread_sqlite(void *arg)
{
	sqlite3 *db;  //数据库操作句柄
	char sql[1024];
	char date[128];


	/*打开数据库*/
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("sqlite3_open error:%s\n", sqlite3_errmsg(db));
	}
	//printf("%s数据库打开成功"， DATABASE);

	/*将环境信息插入数据库*/
	while(1){
		get_date(date);
		char *buf = env_info_struct(&sm_all_env_info, 1);
		sprintf(sql, "insert into env_info values('%s', '%s');", date, buf);
		sleep(10);
	}

	/*查询所有环境信息*/
	do_query(db);


	printf("pthread_sqlite\n");
}

/*模拟数据*/
char * env_info_struct(struct env_info_client_addr *rt_status,int home_id)
{
	int  env_info_size = sizeof(struct env_info_client_addr);
	char *buf = NULL;

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

	sprintf(buf, "temperature = %f\t humidity = %f\t \
		adc = %f\t gyrox = %d\t gyroy = %d\t gyroz = %d\t \
		aacx = %d\t aacy = %d\t aacz = %d\t", \
		rt_status->monitor_no[home_id].zigbee_info.temperature,\
		rt_status->monitor_no[home_id].zigbee_info.humidity,\
		rt_status->monitor_no[home_id].a9_info.adc,\
		rt_status->monitor_no[home_id].a9_info.gyrox,\
		rt_status->monitor_no[home_id].a9_info.gyroy,\
		rt_status->monitor_no[home_id].a9_info.gyroz,\
		rt_status->monitor_no[home_id].a9_info.aacx,\
		rt_status->monitor_no[home_id].a9_info.aacy,\
		rt_status->monitor_no[home_id].a9_info.aacz);

	return buf;
}


/*得到系统时间*/
int get_date(char *date)
{
	time_t t;
	struct tm *tp;

	time(&t);

	//进行时间格式转换
	tp = localtime(&t);
	
	/*将时间装进date*/
	sprintf(date, "%4d-%2d-%2d %2d:%2d:%2d", tp->tm_year + 1900, tp->tm_mon+1, tp->tm_mday, 
			tp->tm_hour, tp->tm_min , tp->tm_sec);

	return 0;
}


int do_query(sqlite3 *db){
	char sql[128] = {};
	char *pzErrmsg;
	char **pazResult;
	int pnRow;
	int pnColumn;
	int i, j;

	sprintf(sql, "select * from env_info;");

	if(sqlite3_get_table(db, sql, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != SQLITE_OK)
	{
		printf("%s\n", pzErrmsg);
	}

	int index = pnColumn;

	for(j = 0; j < pnColumn; j++){
		printf("%-10s", pazResult[j]);
	}
	putchar(10);
	for(i = 0; i < pnRow; i++)
	{
		for(j = 0; j < pnColumn; j++){
			printf("%-10s", pazResult[index++]);
		}
		putchar(10);
	}
	return 0;
}

#endif