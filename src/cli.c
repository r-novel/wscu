#include "cli.h"
#include "log.h"

void tmux(const char* url, char* out) {
	int res = get_tmux(url, out);
	if (res < 0) {
		switch (res) {
			case -1: fprintf(stderr, "[main] error with open output file; error code is: %d\n", res); break;
			case -2: fprintf(stderr, "[main] error with redirect; error code is: %d\n", res); break;
			case -3: fprintf(stderr, "[main] error with curl init; error code is: %d\n", res); break;
			default: break;
		}
	}

	printf("\n");
	INFO("function has been finished with curl code");
	// fprintf(stdout, "[main] function has been finished with curl code: %d\n", res);
}

//TODO: need optimize that shitible code;
void cleaner(char* in) {
	int ok = 1;
	char* out = temp(in);
	if (out) {
		ok = rm_dir(out);
		if (ok == 0) {
			fprintf(stdout, "tmp directory removed successfully;\n");
			return;
		} else {
			ok = rm_dir(temp(DEFAULT_DIR_NAME));
			if (ok == 0) {
				fprintf(stdout, "tmp directory (using default name) removed successfully;\n");
				return;
			} else {
				fprintf(stderr, "tmp directory remove failed;\n");
				return;
			}
		}
	}

	fprintf(stderr, "something wrong with got tmp dir;\n");
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

void info(char* argv, int c) {
	if(c) {
	fprintf(stderr, "%s: invalid option --%c\n", argv, c);
	}
	fprintf(stderr, "Try `%s --help' for more information.\n", argv);
}


void defaultize() {
	char name[FILENAME_MAX];
	char* dir = mk_dir(NULL);
	if (dir) {
		snprintf(name, sizeof(name), "%s/%s", dir, get_name(DEFAULT_TMUX_URL));
	}
	free(dir);
	tmux(DEFAULT_TMUX_URL, name);
}

void customize(int argc, char** argv) {
	char* url = NULL;
	char* nm = NULL;

	int c;
	const char* short_opt = "hmrud:";
	struct option long_opt[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "make", required_argument, NULL, 'm' },
		{ "remove", required_argument, NULL, 'r' },
		{ "url", required_argument, NULL, 'u' },
		{ "dir", required_argument, NULL, 'd' },
		{ NULL, 0, NULL, 0}
	};
	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
	    switch(c) {
				case -1:
				case 0:
				break;

		    case 'm': fprintf(stdout, "directory %s was created;\n", mk_dir(optarg)); return;
		    case 'r': nm = optarg; cleaner(nm); return;
		    case 'u': url = optarg; break;
		    case 'd': nm = optarg; break;
		    case 'h': usage(argv[0]); return;
		    case ':': case '?': info(argv[0], 0); return;
		    default: info(argv[0], c); return;
	    };
	  };

	char name[FILENAME_MAX];
	char* dir = mk_dir(nm);
	if (dir) {
		snprintf(name, sizeof(name), "%s/%s", dir, get_name(url));
	}
	free(dir);
	tmux(url, name);
}