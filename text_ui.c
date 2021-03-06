#include "ui.h"

#define MAX_LEN 128

static const int CTRLS_LEFT  = KEY_LEFT;
static const int CTRLS_BOT   = KEY_DOWN;
static const int CTRLS_TOP   = KEY_UP;
static const int CTRLS_RIGHT = KEY_RIGHT;
static const int CTRLS_OK    = '\n';
static const int CTRLS_QUIT  = 'q';
UI *ui_init() {
	initscr();
	raw();
	noecho();
	curs_set(FALSE);

	//UI struct init.
	UI *ui = (UI*) malloc(sizeof(UI));
	ui->main_win = newwin(LINES, COLS, 0, 0);
	keypad(ui->main_win, TRUE);

	ui->signal = CONTINUE;

	//Colors
	if (has_colors() == FALSE) {
		endwin();

		printf("Your terminal does not support color. Please try a different terminal program.\n");
		exit(1);
	}
	start_color();
	init_pair(EMPTY,    COLOR_WHITE,    COLOR_BLACK);
	init_pair(RED,      COLOR_RED,      COLOR_BLACK);
	init_pair(GREEN,    COLOR_GREEN,    COLOR_BLACK);
	init_pair(YELLOW,   COLOR_YELLOW,   COLOR_BLACK);
	init_pair(BLUE,     COLOR_BLUE,     COLOR_BLACK);
	init_pair(MAGENTA,  COLOR_MAGENTA,  COLOR_BLACK);
	init_pair(CYAN,     COLOR_CYAN,     COLOR_BLACK);
	init_pair(INVALID,  COLOR_BLACK,    COLOR_BLACK);

	//Display
	box(ui->main_win, 0, 0);
	wrefresh(ui->main_win);
	return ui;
}

void ui_terminate(UI *ui) {
	wborder(ui->main_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(ui->main_win);
	delwin(ui->main_win);
	endwin();
}

void ui_clear(UI *ui) {
	wstandend(ui->main_win);
	wclear(ui->main_win);
	box(ui->main_win, 0, 0);
	wrefresh(ui->main_win);
}

typedef struct {
	int y;
	int x;
} Scryx;

static Scryx new_scryx(int y, int x) {
	return (Scryx) { .y = y, .x = x };
}

static Scryx center_coordinates(UI *ui) {
	int h, w;
	getmaxyx(ui->main_win, h, w);
	return new_scryx(h / 2 - 1, w / 2 - 1);
}

static Scryx hex_to_scryx(UI *ui, Hex h) {
	Scryx ctr = center_coordinates(ui);
	return new_scryx(ctr.y + h.r * 2, ctr.x + h.q * 4 + h.r * 2);
}

static void sc_move(UI *ui, Scryx sc) {
	wmove(ui->main_win, sc.y, sc.x);
}

//Board
void print_board(UI *ui, Content **b, const int side) {
	wstandend(ui->main_win);
	int i, j;
	Scryx cell;
	for (i=0; i < boardh(side); i++) {
		for (j=0; j < linew(side, i); j++) {
			Hex h = stor_to_hex(side, new_stor(i, j));
			cell = hex_to_scryx(ui, h);
			sc_move(ui, cell);
			Content ct = get_ct(b, side, h);
			wattron(ui->main_win, COLOR_PAIR(ct));
			if (get_ct(b, side, h) != EMPTY) {
				wattron(ui->main_win, A_BOLD);
				waddch(ui->main_win, 'O');
				wattroff(ui->main_win, A_BOLD);
			} else {
				waddch(ui->main_win, '.');
			}
			wattroff(ui->main_win, COLOR_PAIR(ct));
		}
	}
	wrefresh(ui->main_win);
}

//Cells
void links(UI* ui, const int side, Hex h1, Hex h2, int mode) {

	if (get_zone(side, h1) == NOWHERE || get_zone(side, h2) == NOWHERE) return;

	if (distance(h1, h2) != 1) return;


	Cube c1 = hex_to_cube(h1);
	Cube c2 = hex_to_cube(h2);

	char link_char;
	if (!mode) {
		link_char = ' ';
	} else if (c1.x == c2.x) {
		link_char = '\\';
	} else if (c1.y == c2.y) {
		link_char = '/';
	} else {
		link_char = '-';
	}

	Scryx sc1 = hex_to_scryx(ui, h1);
	Scryx sc2 = hex_to_scryx(ui, h2);

	wmove(ui->main_win, (sc1.y + sc2.y) / 2, (sc1.x + sc2.x) / 2);
	waddch(ui->main_win, link_char);
	wrefresh(ui->main_win);
}

Hex move_cursor(UI* ui, Content** b, const int side, Hex curs_h) {
	int cont = 1;
	Hex next;
	Content ct;
	int ch;
	char *menu_title;
	char *menu_items[] = {};
	int nb_items;
	int menu_res;
	wstandend(ui->main_win);
	while (cont) {

		pid_t pidf = fork();
		if (pidf == 0) {
		sound(cursormove, pidf);
		exit(0);}
		//kill(pidf, SIGTERM);

		sc_move(ui, hex_to_scryx(ui, curs_h));
		ct = get_ct(b, side, curs_h);
		wattrset(ui->main_win, COLOR_PAIR(ct) | A_REVERSE);
		if (ct != EMPTY) {
			waddch(ui->main_win, 'O');
		} else {
			waddch(ui->main_win, '.');
		}
		wrefresh(ui->main_win);

		ch = wgetch(ui->main_win);
		next = curs_h;
		if (ch == CTRLS_RIGHT) {
			next.q++;
		} else if (ch == CTRLS_TOP) {
			next.r--;
			next.q += abs(next.r) % 2;
			if ((get_zone(side, next) == NOWHERE) || (get_ct(b, side, next) == INVALID)) {
				next.q += 1 - 2 * (abs(next.r) % 2);
			}
		} else if (ch == CTRLS_LEFT) {
			next.q--;
		} else if (ch == CTRLS_BOT) {
			next.r++;
			next.q -= 1 - abs(next.r) % 2;
			if ((get_zone(side, next) == NOWHERE) || (get_ct(b, side, next) == INVALID)) {
				next.q += 1 - 2 * (abs(next.r) % 2);
			}
		} else if (ch == CTRLS_OK) {
			cont = 0;
		} else if (ch == CTRLS_QUIT) {
			ui_clear(ui);

			pid_t pidd = fork();
			if (pidd == 0) {
			sound(menu, pidd);
			exit(0);}
			//kill(pidd, SIGTERM);

			menu_title = "Are you sure you want to quit?";
			menu_items[0] = "Go Back to Game"; menu_items[1] = "Quit the Game.";
			nb_items = 2;
			menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
			if (menu_res) {

				pid_t pide = fork();
				if (pide == 0) {
				sound(gameover, pide);
				exit(0);}
				//kill(pide, SIGTERM);

				ui->signal = QUIT;
			} else {
				print_board(ui, b, side);
			}
			cont = 0;
		}

		if ((get_zone(side, next) != NOWHERE) && (get_ct(b, side, next) != INVALID)) {
			sc_move(ui, hex_to_scryx(ui, curs_h));
			wattroff(ui->main_win, A_REVERSE);
			if (ct != EMPTY) {
				wattron(ui->main_win, A_BOLD);
				waddch(ui->main_win, 'O');
			} else {
				wattron(ui->main_win, A_NORMAL);
				waddch(ui->main_win, '.');
			}
			wrefresh(ui->main_win);
			curs_h = next;
		}
	}
	wstandend(ui->main_win);
	return curs_h;
}

//Not board or cell related
int choice_menu(UI *ui, char *title, const int len, char **items) {
	int h = getmaxy(ui->main_win);
	int i, ch;
	Scryx sc;
	sc.y = h / 2 - len;
	sc.x = 2;

	//First display
	sc_move(ui, sc);
	wstandend(ui->main_win);
	wattrset(ui->main_win, A_REVERSE);
	waddstr(ui->main_win, title);
	for (i=0; i < len; i++) {
		wmove(ui->main_win, sc.y + 2 * i + 2, sc.x + 2 * i + 2);
		wattrset(ui->main_win, COLOR_PAIR(1 + i % 6) | A_BOLD);
		waddch(ui->main_win, 'O');
		wattrset(ui->main_win, A_NORMAL);
		wprintw(ui->main_win, " %s", items[i]);
	}

	//User choice
	i = 0;
	wmove(ui->main_win, sc.y + 2, sc.x + 2);
	wchgat(ui->main_win, 1, A_REVERSE, 1 + i % 6, NULL);
	while ((ch = wgetch(ui->main_win)) != CTRLS_OK) {
		wchgat(ui->main_win, 1, A_BOLD, 1 + i % 6, NULL);
		if (ch == CTRLS_TOP) {

			pid_t pidb = fork();
			if (pidb == 0) {
			sound(cursormove, pidb);
			exit(0);}
			//kill(pidb, SIGTERM);

			i = (i > 0) ? i - 1 : len - 1;
		} else if(ch == CTRLS_BOT) {

			pid_t pidc = fork();
			if (pidc == 0) {
			sound(cursormove, pidc);
			exit(0);}
			//kill(pidc, SIGTERM);

			i = (i < len - 1) ? i + 1 : 0;
		}
		wmove(ui->main_win, sc.y + 2 * i + 2, sc.x + 2 * i + 2);
		wchgat(ui->main_win, 1, A_REVERSE, 1 + i % 6, NULL);
	}
	wstandend(ui->main_win);
	ui_clear(ui);
	return i;
}

void ui_prompt_string(UI *ui, char *dst, const char *prompt) {
	ui_clear(ui);
	echo();
	curs_set(TRUE);
	wattrset(ui->main_win, A_REVERSE);
	mvwaddstr(ui->main_win, getmaxy(ui->main_win) / 2, 2, prompt);
	wattrset(ui->main_win, A_BOLD);
	mvwaddstr(ui->main_win, getmaxy(ui->main_win) / 2 + 2, 4, "> ");
	wstandend(ui->main_win);
	wgetstr(ui->main_win, dst);
	noecho();
	curs_set(FALSE);
	ui_clear(ui);
}

void print_status(UI *ui, Content ct, char *name) {

	pid_t pida = fork();
	if (pida == 0) {
	sound(next, pida);
	exit(0);}
	//kill(pida, SIGTERM);

	wstandend(ui->main_win);
	mvwhline(ui->main_win, 2, 1, ' ', getmaxx(ui->main_win) - 2);
	wattrset(ui->main_win, A_BOLD);
	mvwprintw(ui->main_win, 2, 2, "Current player: ");
	wattron(ui->main_win, COLOR_PAIR(ct));
	wprintw(ui->main_win, name);
	wstandend(ui->main_win);
}

void disp_msg(UI *ui, Content ct, char *msg) {
	//Wrong input
	if (ct == EMPTY) return;

	//Correct input
	ui_clear(ui);
	wattrset(ui->main_win, A_REVERSE);
	mvwaddstr(ui->main_win, getmaxy(ui->main_win) / 2, 2, "Game Over:");
	wattrset(ui->main_win, A_BOLD);
	mvwaddstr(ui->main_win, getmaxy(ui->main_win) / 2 + 2, 4, "> ");
	wattrset(ui->main_win, COLOR_PAIR(ct));
	mvwaddstr(ui->main_win, getmaxy(ui->main_win) / 2 + 2, 6, msg);
	wgetch(ui->main_win);
	wstandend(ui->main_win);
	ui_clear(ui);}
