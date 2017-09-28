
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "log.h"

int main()
{
	
	char tm[100] = {0};
	char*p = tm;
	gettime(p);
	printf("%s : len = %d\n",tm,strlen(tm));

	return 0;
}
