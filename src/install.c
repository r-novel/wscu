#include "install.h"

int configure(const char* path, char* const argv[]) {
	execv(path, argv);
	return 1;
}

int install(const char* path, char* const argv[]) {
	execv(path, argv);
	return 1;
}