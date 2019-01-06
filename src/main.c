#include <ncurses.h>
#include <string.h> 
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

int main(int argc, char** argv) {
	char mesg[] = "Hello world!";
	int row, col;				
	initscr();				
	getmaxyx(stdscr, row, col);		
	mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
	tmux();
	mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);
	printw("Try resizing your window(if possible) and then run this program again");
	refresh();
	getch();
	endwin();
	
	return 0;
}
