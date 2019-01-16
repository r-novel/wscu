#ifndef _GET_TMUX_H_
#define _GET_TMUX_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include "tmp-dir.h"


struct tmux {
	const char* filename;
	FILE* stream;
};

char* get_home_dir();
char* get_tmp_dir(char*);
int create_dir(char*);

size_t tmux_fwrite(void*, size_t, size_t, void*);
char* get_name(const char*);
int get_tmux(const char*, char*);

#endif // _GET_TMUX_H_