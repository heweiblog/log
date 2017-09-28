#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "log.h"


void gettime(char *pszTimeStr)
{
		struct tm      tSysTime     = {0};
		struct timeval tTimeVal     = {0};
		time_t         tCurrentTime = {0};

		char szUsec[20] = {0};    // 微秒
		char szMsec[20] = {0};    // 毫秒

		if (pszTimeStr == NULL)
		{
				return;
		}

		tCurrentTime = time(NULL);
		localtime_r(&tCurrentTime, &tSysTime);   // localtime_r是线程安全的

		gettimeofday(&tTimeVal, NULL);    
		sprintf(szUsec, "%06lu", tTimeVal.tv_usec);  // 获取微秒
		strncpy(szMsec, szUsec, 3);                // 微秒的前3位为毫秒(1毫秒=1000微秒)

		sprintf(pszTimeStr, "[%04d.%02d.%02d %02d:%02d:%02d.%3.3s]", 
						tSysTime.tm_year+1900, tSysTime.tm_mon+1, tSysTime.tm_mday,
						tSysTime.tm_hour, tSysTime.tm_min, tSysTime.tm_sec, szMsec);
}


int log_add(int level,int type,const char *logfile,const char * fmt,...)
{
	extern dial_cfg_t g_cfg;
	extern bool s_debug_switch;
	extern pthread_mutex_t g_log_lock;
	unsigned char 	buf[MAX_DEBUG_BUFFER_LEN];
    	int 			size;
	va_list 		args;
	unsigned char	filename[128] = {0};

	if(LOG_RUN == type) {
		if(!s_debug_switch || g_cfg.log_level < level)
			return 0;
	}

	memset(buf, 0, MAX_DEBUG_BUFFER_LEN);

	va_start(args, fmt);
	size = vsnprintf((char *)buf, MAX_DEBUG_BUFFER_LEN-1, (char *)fmt, args);
	va_end (args);
	
	//printf("%s", buf);

	if(size > 0) {
		sprintf((char *)filename, "%s%s", g_cfg.log_path,
			logfile);
		pthread_mutex_lock(&g_log_lock);
		// Ð´ÈëÏàÓ¦ÈÕÖ¾ÎÄ¼þ
		write_debug_file(filename, buf);
		pthread_mutex_unlock(&g_log_lock);
	}

	return 0;
}


int
write_debug_file(unsigned char			*filename,
					unsigned char			*buffer)
{
	FILE			*fp = NULL;
	time_t 			timep;
	struct tm 		st_tm;
	int 	ui_size = 0;
	unsigned char 	log_time[30] = { 0 };
	unsigned char 	file_path[50] = { 0 };

	if(!filename || !buffer || strlen((char *)buffer) >= MAX_DEBUG_BUFFER_LEN){

		printf("write_debug_file:invalid parameter!\n");
		return -1;
	}
	
	fp = fopen((char *)filename, "a+");
	if(NULL == fp){

		printf("write_debug_file:open debug file failed<%s>,errno=%d\n", filename,errno);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	ui_size = ftell(fp);
	if(MAX_DEBUG_FILE_SIZE <= ui_size)
	{
		fclose(fp);
		/*Ôö¼ÓÈÕÖ¾ÎÄ¼þºó×ºÃû*/
		strcat((char *)file_path, (char *)filename);
		strcat((char *)file_path, ".bak");
		rename((char *)filename, (char *)file_path);
		fp = fopen((char *)filename, "a+");
		if(!fp){
			return -1;
		}
	}

	time(&timep);
	localtime_r(&timep, &st_tm);
	sprintf((char *)log_time, "%02d-%02d-%02d %02d:%02d:%02d  ",
		(1900 + st_tm.tm_year),
		(1 + st_tm.tm_mon),
		st_tm.tm_mday,
		st_tm.tm_hour,
		st_tm.tm_min,
		st_tm.tm_sec);

	// Ð´Èëµ½ÎÄ¼þÖÐ
	fwrite((char *)log_time, strlen((char *)log_time), 1, fp) ;
	fwrite((char *)buffer, strlen((char *)buffer), 1, fp) ;

	fclose(fp);
	return 0;
}


