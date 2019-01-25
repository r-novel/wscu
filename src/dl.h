#ifndef _DOWNLOADER_H_
#define _DOWNLOADER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include "dir.h"
#include "log.h"


struct tool_t {
	const char* filename;
	FILE* stream;
};

size_t tool_fwrite(void*, size_t, size_t, void*);
char* tool_name(const char*);
int tool(const char*, char*);

#endif // _DOWNLOADER_H_