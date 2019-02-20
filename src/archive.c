#include "archive.h"

int unpack(const char* fname, const char* dest) {
	TAR* tar;
	if (tar_open(tar, fname, NULL, O_RDONLY, 0644, TAR_GNU) != 0) {
		log(error, "fail to open tar file");
		return -1;
	}
	if (tar_extract_all(tar, dest) != 0) {
		log(error, "fail to extract tar file");
		return -1;
	}

	tar_close(tar);
	return 1;
}
