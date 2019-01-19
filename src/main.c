#include <ncurses.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "get-tmux.h"
#include "const.h"

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

	fprintf(stdout, "[main] function has been finished with curl code: %d\n", res);
}


int remove_dir(char* in) {
	DIR* dir = opendir(in);
	size_t in_len = strlen(in);
	int ok = -1;
	
	if (dir) {
		struct dirent* p;
		ok = 0;

		while (!ok && (p = readdir(dir))) {
			int tmp_ok = -1;
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
						tmp_ok = remove_dir(buf);
					else
						tmp_ok = unlink(buf);
				}
				free(buf);
			}
			ok = tmp_ok;
		}

		closedir(dir);
	}

	if (!ok)
		ok = rmdir(in);

	return ok;
}


//TODO: need optimize that shitible code;
void cleaner(char* in) {
	int ok = 1;
	char* out = get_tmp_dir(in);
	if (out) {
		ok = remove_dir(out);
		if (ok == 0) {
			fprintf(stdout, "tmp directory removed successfully;\n");
			return;
		} else {
			ok = remove_dir(get_tmp_dir(DEFAULT_DIR_NAME));
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

char* make_dir(char* name) {
	char* nm;
	if (name)
		nm = get_tmp_dir(name);
	else
		nm = get_tmp_dir(DEFAULT_DIR_NAME);

	if (nm) {
		int ok = create_dir(nm);
		if (ok)
			return nm;
		else 
			return NULL;
	}
	return NULL;
}


void defaultize() {
	char name[FILENAME_MAX];
	char* dir = make_dir(NULL);
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

		    case 'm': fprintf(stdout, "directory %s was created;\n", make_dir(optarg)); return;
		    case 'r': nm = optarg; cleaner(nm); return;
		    case 'u': url = optarg; break;
		    case 'd': nm = optarg; break;
		    case 'h': usage(argv[0]); return;
		    case ':': case '?': info(argv[0], 0); return;
		    default: info(argv[0], c); return;
	    };
	  };

	char name[FILENAME_MAX];
	char* dir = make_dir(nm);
	if (dir) {
		snprintf(name, sizeof(name), "%s/%s", dir, get_name(url));
	}
	free(dir);
	tmux(url, name);
}

void Do(int argc, char** argv) {
	if (argc < 2) {
		defaultize();
	} else {
		customize(argc, argv);
	}
}

int main(int argc, char** argv) {
	
	// char mesg[] = "Hello world!";
	// int row, col;
	// initscr();
	// getmaxyx(stdscr, row, col);
	// mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
	Do(argc, argv);
	// mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);
	// printw("Try resizing your window(if possible) and then run this program again");
	// refresh();
	// getch();
	// endwin();

	return 0;
}
