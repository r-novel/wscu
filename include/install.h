#ifndef _INSTALL_H_
#define _INSTALL_H_

#include <stdio.h>
#include <unistd.h>
#include "dir.h"
#include "const.h"
#include "log.h"

int configure(const char* path, char* const argv[]);
int install(const char* path, char* const argv[]);

#endif // _INSTALL_H_
