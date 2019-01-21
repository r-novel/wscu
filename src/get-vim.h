#ifndef _GET_VIM_H_
#define _GET_VIM_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include "dir.h"
#include "log.h"


struct vim {
	const char* filename;
	FILE* stream;
};

size_t vim_fwrite(void*, size_t, size_t, void*);
char* get_vim_name(const char*);
int get_vim(const char*, char*);

#endif // _GET_VIM_H_