#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#include "data_struct.h"
#include "text_ui.h"
typedef struct UI UI;
UI *ui_init();
void ui_terminate(UI *ui);
void ui_clear(UI *ui);
void print_board(UI *ui, Content **b, const int side);
void links(UI *ui, const int side, Hex h1, Hex h2, int mode);
Hex move_cursor(UI *ui, Content **b, const int side, Hex curs_h);
int choice_menu(UI *ui, char *title, const int len, char **items);
void ui_prompt_string(UI *ui, char *dst, const char *prompt);
void print_status(UI *ui, Content ct, char *name);
void disp_msg(UI *ui, Content ct, char *msg);
#endif
