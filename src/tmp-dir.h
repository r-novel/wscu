#ifndef _GET_TMUX_H_
#define _GET_TMUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

extern char* get_home_dir();
extern char* get_tmp_dir(char*);
extern int create_dir(char*);

#endif // _GET_TMUX_H_