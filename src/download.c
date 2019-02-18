#include "download.h"

size_t tool_fwrite(void *buf, size_t size, size_t nmemb, void* stream) {
	struct tool_t* out = (struct tool_t*)stream;
	if(out && !out->stream) {
		out->stream = fopen(out->filename, "w");
		if(!out->stream)
			return -1;
  }
	return fwrite(buf, size, nmemb, out->stream);
}

char* tool_name(const char* url) {
	if(url) {
		char* c = strrchr(url, '/');
		if (c)
			return c + 1;
		else {
			log(error, " error with get token;\n");
			return NULL;
		}
	}
	return NULL;
}

int tool(const char* url, char* outname) {
	CURL* curl;
	char* location;
	long resp_code;
	CURLcode res;
	
	struct tool_t out = {outname, NULL};
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			log(error, " error with curl perform; raw error: %s\n", curl_easy_strerror(res));
		else {
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp_code);
			if((res == CURLE_OK) && ((resp_code / 100) != 3)) {
				log(error, "not a redirect;\n");
				return -2;
			} else {
				res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
				if((res == CURLE_OK) && location) {
					curl_easy_setopt(curl, CURLOPT_URL, location);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, tool_fwrite);
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