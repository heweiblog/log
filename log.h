#ifndef _LOG_H_
#define _LOG_H_

enum log_type
{
	LOG_CONFIG,
	LOG_ERROR,
	LOG_DEBUG,
	LOG_INFO,
};

enum lo_level 
{
	LEVEL_CONFIG,
	LEVEL_ERROR,
	LEVEL_DEBUG,
	LEVEL_INFO,
};


int log_add(int level,int type,const char *logfile,const char * fmt,...);

int write_debug_file(unsigned char* filename,unsigned char* buffer);

#define LOG_FILE "dial.log"

#define	LOG(level,fmt,...)  log_add(level,LOG_CONFIG,LOG_FILE,fmt,##__VA_ARGS__)




#endif
