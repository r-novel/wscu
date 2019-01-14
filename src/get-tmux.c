#include "get-tmux.h"

size_t tmux_fwrite(void *buf, size_t size, size_t nmemb, void* stream) {
	struct tmux* out = (struct tmux*)stream;
	if(out && !out->stream) {
		out->stream = fopen(out->filename, "w");
		if(!out->stream)
			return -1;
  }
	return fwrite(buf, size, nmemb, out->stream);
}

char* get_name(const char* url) {
	if (url) {
		char* c = strrchr(url, '/');
		if (c)
			return c + 1;
		else {
			fprintf(stderr, "[get-name] error with get token;\n");
			return NULL;
		}
	}
	return NULL;
}

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
		snprintf(name, FILENAME_MAX * sizeof(char), "%s/%s", h, ".wscu_tmp");
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

int get_tmux(const char* url) {
	CURL* curl;
	char* location;
	long resp_code;
	CURLcode res;
	char name[FILENAME_MAX];
	int ok = create_dir();
	char* nm = get_tmp_dir();
	if (ok >= 0) {
		snprintf(name, sizeof(name), "%s/%s", nm, get_name(url));
	}
	free(nm);

	struct tmux out = {name, NULL};
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "[get-tmux] error with curl perform; raw error: %s\n", curl_easy_strerror(res));
		else {
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp_code);
			if((res == CURLE_OK) && ((resp_code / 100) != 3)) {
				fprintf(stderr, "not a redirect;\n");
				return -2;
			} else {
				res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
				if((res == CURLE_OK) && location) {
					curl_easy_setopt(curl, CURLOPT_URL, location);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, tmux_fwrite);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
					curl_easy_perform(curl);
        }
      }
		}
				
		if (out.stream)
			fclose(out.stream);

		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return res;
	}
	return -3;
}

