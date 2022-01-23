#include <ncurses.h>


struct UI {
	enum { CONTINUE, QUIT } signal;
	WINDOW *main_win;};
