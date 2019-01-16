#include "tmp-dir.h"

char* get_home_dir() {
	struct passwd* pw = getpwuid(getuid());
	char* home = pw->pw_dir;
	if (home) {
		return home;
	}
	return NULL;
}

char* get_tmp_dir() {
	char* name = (char*) malloc(FILENAME_MAX * sizeof(char));
	if (name == NULL) {
		return NULL;
	}
	char* h = get_home_dir();
	if (h) {
		snprintf(name, FILENAME_MAX * sizeof(char), "%s/%s", h, ".wscu");
		return name;
	}
	return NULL;
}

int create_dir() {
	struct stat st = {0};
	char* t = get_tmp_dir();
	if (t) {
		if (stat(t, &st) == -1) {
			mkdir(t, 0777);
			return 1;
		} else {
			return 0;
		}
	}
	return -1;
}