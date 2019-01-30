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
	fprintf(stdout, "\t-c --config\t\tconfig file for download tools;\n");
	fprintf(stdout, "\t--configure\t\toption for generate workflow config file;\n");
	fprintf(stdout, "\t-r --remove\t\tremove temporary directory;\n");
	fprintf(stdout, "\t-d --dir\t\ttemporary directory name;\n");
	fprintf(stdout, "\t-h, --help\t\tprint usage information;\n");
}

void err_msg(char* argv, int c) {
	if(c) {
	fprintf(stderr, "%s: invalid option --%c\n", argv, c);
	}
	fprintf(stderr, "Try `%s --help' for more information.\n", argv);
}

/* 
TODO rework or delete that function;
What do with struct cfg_tool? However, field name need to create humanize configuration file;
Maybe field name will be delete;
*/
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
	char* fname = NULL;
	char* dir = NULL;
	struct cfg_tool tool[2];
	int c;
	const char* short_opt = "hmrdc:";
	struct option long_opt[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "make", required_argument, NULL, 'm' },
		{	"config", required_argument, NULL, 'c' },
		{ "configure", no_argument, NULL, 1 },
		{ "remove", required_argument, NULL, 'r' },
		{ "dir", required_argument, NULL, 'd' },
		{ NULL, 0, NULL, 0}
	};
	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
	    switch(c) {
				case -1: case 0: break;
		    case 'm': log(info, "directory %s was created;\n", dir = mk_dir(optarg)); break;
		    case 'r': nm = optarg; cleaner(nm); return;
		    case 'c': fname = optarg; cfg_tool(fname, tool); break;
		    case 'd': nm = optarg; break;
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
	
	if (!dir) {
		log(info, "temp dir do not created;");
		dir = mk_dir(NULL);
	}
	for (int i = 0; i < 2; ++i) {
		snprintf(name, sizeof(name), "%s/%s", dir, tool_name(tool[i].url));
		download(tool[i].url, name);
	}

	for(int i = 0; i < 2; ++i) {
		cfg_tool_free(&tool[i]);
	}
	free(dir);
	log(info, "download successfully;");
}