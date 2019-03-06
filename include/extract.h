#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <archive.h>
#include <archive_entry.h>
#include "dir.h"
#include "const.h"
#include "log.h"

int extract(const char*);

#endif // _ARCHIVE_H_