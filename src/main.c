#include <ncurses.h>
#include <string.h>
#include <getopt.h>
#include "get-tmux.h"
 
const char* url = "https://github.com/tmux/tmux/releases/download/2.8/tmux-2.8.tar.gz";

void tmux() {
	int res = get_tmux(url);
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

int cleaner() {
	
}

void usage(char* argv) {
	fprintf(stdout, "Usage: %s [OPTIONS]\n", argv);
	fprintf(stdout, "\t-c --clean\t\tclean tmp dot directory;\n");
	fprintf(stdout, "\t-h, --help\t\tprint usage message and exit;\n");
}

void info(char* argv, int c) {
	if(c) {
	fprintf(stderr, "%s: invalid option -- %c\n", argv, c);
	}
	fprintf(stderr, "Try `%s --help' for more information.\n", argv);
}

int main(int argc, char** argv) {
	int c;
	const char* short_opt = "hc:";
	struct option long_opt[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "clean", required_argument, NULL, 'c'},
		{ NULL, 0, NULL, 0}
	};

	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
    switch(c) {
			case -1:
			case 0:
			break;

	    case 'c':
	    	printf("you entered \"%s\"\n", optarg);
	    break;

	    case 'h': usage(argv[0]); return 0;

	    case ':':
	    case '?':
				info(argv[0], 0);
	    return 2;

	    default:
	    	info(argv[0], c);
	    return 2;
    };
  };


	// char mesg[] = "Hello world!";
	// int row, col;
	// initscr();
	// getmaxyx(stdscr, row, col);
	// mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
	// tmux();
	// mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);
	// printw("Try resizing your window(if possible) and then run this program again");
	// refresh();
	// getch();
	// endwin();

	return 0;
}
