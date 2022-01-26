// _________ .__    .__
// \_   ___ \|  |__ |__| ____   ____   ______ ____
// /    \  \/|  |  \|  |/    \_/ __ \ /  ___// __ \
// \     \___|   Y  \  |   |  \  ___/ \___ \\  ___/
//  \______  /___|  /__|___|  /\___  >____  >\___  >
//         \/     \/        \/     \/     \/     \/
// _________ .__                   __
// \_   ___ \|  |__   ____   ____ |  | __ ___________  ______
// /    \  \/|  |  \_/ __ \_/ ___\|  |/ // __ \_  __ \/  ___/
// \     \___|   Y  \  ___/\  \___|    <\  ___/|  | \/\___ \
//  \______  /___|  /\___  >\___  >__|_ \\___  >__|  /____  >
//         \/     \/     \/     \/     \/    \/           \/

#include <stdlib.h>
#include <string.h>

#include "data_struct.h"
#include "ui.h"
#include "game.h"

void print_image(FILE *fptr);

#define MAX_LEN 128;
#define DEF_BOARD_SIDE 5;

#ifdef __linux__
	char gameover[] = "aplay sounds/gameover.mp3";
	char menu[] = "aplay sounds/menu.wav";
	char cursormove[] = "aplay sounds/move.wav";
	char next[] = "aplay sounds/next.wav";
#endif

#ifdef __APPLE__
	char gameover[] = "afplay sounds/gameover.mp3";
	char menu[] = "afplay sounds/menu.wav";
	char cursormove[] = "afplay sounds/move.wav";
	char next[] = "afplay sounds/next.wav";
#endif

int main(int argc, char* argv[]){
	system("clear");

	UI* ui = ui_init();
	int side = DEF_BOARD_SIDE;
	if(argc == 2){
		side = strtol(argv[1], NULL, 10);}
	Content** b;

	pid_t pidi = fork();
	if (pidi == 0) {
	sound(menu, pidi);
	exit(0);}
	//kill(pidi, SIGTERM);
	//system(menu);

	char* menu_title = "Chinese Checkers"; char* menu_items[] = {"Play", "Quit"};
	int menu_len = sizeof(menu_items)/sizeof(menu_items[0]);
	while((ui->signal = choice_menu(ui, menu_title, menu_len, menu_items) == 0 ? CONTINUE : QUIT) != QUIT) {
		b = init_board(side);

		int nb_players;
		Player* cur_plr = init_players(ui, &nb_players);
		int i;
		for(i=0; i<nb_players; i++){
			set_corner_ct(b, side, get_opposite(cur_plr->goal), cur_plr->ct);
			if (nb_players == 3)
				set_corner_ct(b, side, cur_plr->goal, EMPTY);
			cur_plr = cur_plr->next;}

		print_board(ui, b, side);
		print_status(ui, cur_plr->ct, cur_plr->name);
		while(ui->signal != QUIT){
			play_turn(ui, b, side, cur_plr);
			if(has_won(b, side, cur_plr)){
				//GAME OVER OCCURS HERE

				pid_t pidh = fork();
				if (pidh == 0) {
				sound(gameover, pidh);
				exit(0);}
				//system(gameover);

				char msg[20] = "";
				sprintf(msg, "%s wins!", cur_plr->name);
				disp_msg(ui, cur_plr->ct, msg);
				ui->signal = QUIT;}
			cur_plr = cur_plr->next;
			print_board(ui, b, side);
			print_status(ui, cur_plr->ct, cur_plr->name);}
		ui_clear(ui);
		free(b);}

ui_terminate(ui);
return EXIT_SUCCESS;}
