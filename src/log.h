#include <stdio.h>

#define INFO(msg) \
	fprintf(stdout, "\033[1;34m[%s:%s][%s][%s():%d] ", __DATE__, __TIME__, __FILE__, __func__, __LINE__); \
	fprintf(stdout, "%s\n\033[0m", msg);
