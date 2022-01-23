#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <stdlib.h>
#include <string.h>

#include "data_struct.h"
#include "ui.h"
typedef struct player_struct {
	Content ct;
	Zone goal;
	Hex curs_h;
	char name[20];
	struct player_struct *next;
} Player;
Player *init_players(UI *ui, int *nb_players);
void move_piece(Content **b, const int side, Hex from, Hex to);
void link_accessible_from(UI *ui, Content **b, const int side, int *nb, Hex cells[], int mode);
void play_turn(UI *ui, Content **b, const int side, Player *plr);
int has_won(Content **b, const int side, Player *plr);
#endif
