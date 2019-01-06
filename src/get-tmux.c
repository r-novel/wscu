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
	char* location;
	long resp_code;
	CURLcode res;

	struct tmux out = {(const char*)get_name(url), NULL};
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
		curl_easy_cleanup(curl);		
		if (out.stream)
			fclose(out.stream);
	  curl_global_cleanup();

		return res;
	}
	return -3;
}

