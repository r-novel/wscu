#include "cli.h"

void download(const char* url, char* out) {
	int res = tool(url, out);
	if (res < 0) {
		switch (res) {
			case -1: log(error, " error with open output file; error code is: %d\n", res); break;
			case -2: log(error, " error with redirect; error code is: %d\n", res); break;
			case -3: log(error, " error with curl init; error code is: %d\n", res); break;
			default: break;
		}
	}

	log(info, "function has been finished with curl code: %d", res);
}

//TODO: need optimize that shitible code;
void cleaner(char* in) {
	int ok = 1;
	char* out = temp(in);
	if (out) {
		ok = rm_dir(out);
		if (ok == 0) {
			log(info, "tmp directory removed successfully;\n");
			return;
		} else {
			ok = rm_dir(temp(DEFAULT_DIR_NAME));
			if (ok == 0) {
				log(warning, "tmp directory (using default) removed successfully;\n");
				return;
			} else {
				log(error, "tmp directory remove failed;\n");
				return;
			}
		}
	}

	log(error, "something wrong with got tmp dir;\n");
	return;
}

void usage(char* argv) {
	fprintf(stdout, "Usage: %s [OPTIONS]\n", argv);
	fprintf(stdout, "\t-m --make\t\tmake temporary directory for contain source codes;\n");
	fprintf(stdout, "\t-r --remove\t\tremove temporary directory;\n");
	fprintf(stdout, "\t-u --url\t\tinput url for downloading utilities;\n");
	fprintf(stdout, "\t-d --dir\t\ttemporary directory name;\n");
	fprintf(stdout, "\t-h, --help\t\tprint usage information;\n");
}

void err_msg(char* argv, int c) {
	if(c) {
	fprintf(stderr, "%s: invalid option --%c\n", argv, c);
	}
	fprintf(stderr, "Try `%s --help' for more information.\n", argv);
}


void defaultize() {
	char tmux[FILENAME_MAX];
	char vim[FILENAME_MAX];
	char* dir = mk_dir(NULL);
	if (dir) {
		snprintf(tmux, sizeof(tmux), "%s/%s", dir, tool_name(DEFAULT_TMUX_URL));
		snprintf(vim, sizeof(vim), "%s/%s", dir, tool_name(DEFAULT_VIM_URL));
	}
	free(dir);
	download(DEFAULT_TMUX_URL, tmux);
	download(DEFAULT_VIM_URL, vim);
}

void customize(int argc, char** argv) {
	char* nm = NULL;
	int c;
	const char* short_opt = "hmrd:";
	struct option long_opt[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "make", required_argument, NULL, 'm' },
		{ "remove", required_argument, NULL, 'r' },
		{ "dir", required_argument, NULL, 'd' },
		{ NULL, 0, NULL, 0}
	};
	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
	    switch(c) {
				case -1: case 0: break;
		    case 'm': log(info, "directory %s was created;\n", mk_dir(optarg)); return;
		    case 'r': nm = optarg; cleaner(nm); return;
		    case 'd': nm = optarg; break;
		    case 'h': usage(argv[0]); return;
		    case ':': case '?': err_msg(argv[0], 0); return;
		    default: err_msg(argv[0], c); return;
	    };
	  };

	defaultize();
}