#include <ncurses.h>

#include "cli.h"
#include <stdio.h>

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
