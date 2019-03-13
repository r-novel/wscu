#include "cli.h"

void download(const char* url, char* out) {
	int res = tool(url, out);
	if (res < 0) {
		switch (res) {
			case -1: log(error, " error with open output file; error code is: %d", res); break;
			case -2: log(error, " error with redirect; error code is: %d", res); break;
			case -3: log(error, " error with curl init; error code is: %d", res); break;
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
			log(info, "tmp directory removed successfully;");
			return;
		} else {
			ok = rm_dir(temp(DEFAULT_DIR_NAME));
			if (ok == 0) {
				log(warning, "tmp directory (using default) removed successfully;");
				return;
			} else {
				log(error, "tmp directory remove failed;");
				return;
			}
		}
	}

	log(error, "something wrong with got tmp dir;");
	return;
}

void usage(char* argv) {
	fprintf(stdout, "Usage: %s [OPTIONS]\n", argv);
	fprintf(stdout, "\t-c --config\t\tconfig file for download tools;\n");
	fprintf(stdout, "\t-r --remove\t\tremove temporary directory;\n");
	fprintf(stdout, "\t--configure\t\toption for generate workflow config file;\n");
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
	char* fname = NULL;
	char* dir = NULL;
	
	struct cfg_tool tool[3];
	int c;
	const char* short_opt = "hmrdc:";
	struct option long_opt[] = {
		{ "help", no_argument, NULL, 'h' },
		{	"config", required_argument, NULL, 'c' },
		{ "remove", no_argument, NULL, 'r'},
		{ "configure", no_argument, NULL, 1 },
		{ NULL, 0, NULL, 0}
	};
	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
	    switch(c) {
				case -1: case 0: break;
		    case 'r': cleaner(NULL); return;
		    case 'c': fname = optarg; cfg_tool(fname, tool); break;
		    case 'h': usage(argv[0]); return;
		    case ':': case '?': err_msg(argv[0], 0); return;
		    case 1: 
		    	log(trace, "Will be added config maker here;"); 
		    	struct cfg_tool t[] = { 
		    		{ "fzf", "github.com/fzf/fzf.tar.gz" }, 
		    		{ "mutt", "github.com/mutt/mutt.tar.gz" },
		    	};
		    	perform_generate(t); 
		    return;
		    default: err_msg(argv[0], c); return;
	    };
	  };

	char name[FILENAME_MAX];

	cleaner(NULL);
	log(info, "temp dir do not created;");
	dir = mk_dir(NULL);
	
	for (int i = 0; i < 3; ++i) {
		snprintf(name, sizeof(name), "%s/%s", dir, tool_name(tool[i].url));
		download(tool[i].url, name);
		extract(name);
	}

	char test_exec[FILENAME_MAX];
	snprintf(test_exec, sizeof(test_exec), "%s/%s", dir, TEST_NAME);
	configure(test_exec, NULL);

	for(int i = 0; i < 3; ++i) {
		cfg_tool_free(&tool[i]);
	}
	free(dir);
	log(info, "download successfully;");
}