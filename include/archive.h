#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <libtar.h>
#include "log.h"

int unpack(char*, char*);

#endif // _ARCHIVE_H_