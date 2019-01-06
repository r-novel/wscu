#ifndef _GET_TMUX_H_
#define _GET_TMUX_H_

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

struct tmux {
	const char* filename;
	FILE* stream;
};

size_t tmux_fwrite(void* buf, size_t size, size_t nmemb, void* stream);
char* get_name(const char* url);
int get_tmux(const char* url);

#endif // _GET_TMUX_H_