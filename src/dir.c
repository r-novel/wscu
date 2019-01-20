#include "dir.h"

char* home() {
	struct passwd* pw = getpwuid(getuid());
	char* home = pw->pw_dir;
	if (home) {
		return home;
	}
	return NULL;
}

char* temp(char* in) {
	char* name = (char*) malloc(FILENAME_MAX * sizeof(char));
	if (name == NULL) {
		return NULL;
	}
	char* dir = home();
	if (dir) {
		snprintf(name, FILENAME_MAX * sizeof(char), "%s/%s", dir, in);
		return name;
	}
	return NULL;
}

int create(char* in) {
	struct stat st = { 0 };
	if (in) {
		if (stat(in, &st) == -1) {
			mkdir(in, 0777);
			return 1;
		} else {
			return 0;
		}
	}
	return -1;
}

char* mk_dir(char* name) {
	char* nm;
	if (name)
		nm = temp(name);
	else
		nm = temp(DEFAULT_DIR_NAME);

	if (nm) {
		int ok = create(nm);
		if (ok)
			return nm;
		else 
			return NULL;
	}
	return NULL;
}

int rm_dir(char* in) {
	DIR* dir = opendir(in);
	size_t in_len = strlen(in);
	int res = -1;
	
	if (dir) {
		struct dirent* p;
		res = 0;

		while (!res && (p = readdir(dir))) {
			int tmp_res = -1;
			char* buf = NULL;
			size_t len;

			if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
				continue;

			len = in_len + strlen(p->d_name) + 2; 
			buf = (char*)malloc(len);

			if (buf) {
				struct stat statbuf;
				snprintf(buf, len, "%s/%s", in, p->d_name);

				if (!stat(buf, &statbuf)) {
					if (S_ISDIR(statbuf.st_mode))
						tmp_res = rm_dir(buf);
					else
						tmp_res = unlink(buf);
				}
				free(buf);
			}
			res = tmp_res;
		}

		closedir(dir);
	}

	if (!res)
		res = rmdir(in);

	return res;
}
