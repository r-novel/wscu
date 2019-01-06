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

int get_tmux(const char* url) {
	CURL* curl;
	CURLcode res;
	struct tmux out = {(const char*)get_name(url), NULL};
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, tmux_fwrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK)
			fprintf(stderr, "[get-tmux] error with curl perform; res code: %d\n", res);
		
		if (out.stream)
			fclose(out.stream);

	  curl_global_cleanup();
		return res;
	}

	return -2;
}

