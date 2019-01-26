#ifndef _DIR_H_
#define _DIR_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#include "const.h"

extern char* home();
extern char* temp(char*);
extern int create(char*);

extern char* mk_dir(char*);
extern int rm_dir(char*);

#endif // _DIR_H_