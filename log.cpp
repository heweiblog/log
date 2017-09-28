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

int log_error(char*data)
{
}

int main()
{
	
	char tm[100] = {0};
	char*p = tm;
	gettime(p);
	printf("%s : len = %d\n",tm,strlen(tm));
}
