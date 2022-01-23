#include "game.h"

static void init_player_color(UI *ui, Player *plr, Content availables[6]);

Player* init_players(UI* ui, int* nb_players){

	pid_t pidj = fork();
	if (pidj == 0) {
	sound(menu, pidj);
	exit(0);}
	//kill(pidj, SIGTERM);

	char* menu_title = "Select the Number of Players"; char* menu_items[] = {"2", "3", "4", "6"};
	int nb_items = 4;
	int menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
	*nb_players = 2+menu_res+(menu_res == 3);

	Zone goals[*nb_players];
	if(*nb_players == 2){
		goals[0] = TOP;
		goals[1] = BOT;}
	else if(*nb_players == 3){
		goals[0] = TOP;
		goals[1] = BOT_LEFT;
		goals[2] = BOT_RIGHT;}
	else if(*nb_players == 4){
		goals[0] = TOP_LEFT;
		goals[1] = BOT_LEFT;
		goals[2] = BOT_RIGHT;
		goals[3] = TOP_RIGHT;}
	else{
		goals[0] = TOP;
		goals[1] = TOP_LEFT;
		goals[2] = BOT_LEFT;
		goals[3] = BOT;
		goals[4] = BOT_RIGHT;
		goals[5] = TOP_RIGHT;}

	Content availables[6] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
	Player *p1 = (Player*) malloc(sizeof(Player));
	p1->goal = goals[0]; p1->curs_h = new_hex(0, 0);

	pid_t pidk = fork();
	if (pidk == 0) {
	sound(menu, pidk);
	exit(0);}
	//kill(pidk, SIGTERM);

	ui_prompt_string(ui, p1->name, "First Player Name:");
	init_player_color(ui, p1, availables);

	Player *cur, *prev = p1;
	int i;
	for(i=1; i<*nb_players; i++){
		cur = (Player*) malloc(sizeof(Player));
		prev->next = cur; cur->goal = goals[i]; cur->curs_h = new_hex(0, 0);

		pid_t pidl = fork();
		if (pidl == 0) {
		sound(menu, pidl);
		exit(0);}
		//kill(pidl, SIGTERM);

		ui_prompt_string(ui, cur->name, "Next Player Name:");
		init_player_color(ui, cur, availables);
		prev = cur;}
	cur->next = p1;
	return p1;}

static void init_player_color(UI *ui, Player *plr, Content availables[6]){
	char menu_title[50];

	pid_t pidm = fork();
	if (pidm == 0) {
	sound(menu, pidm);
	exit(0);}
	//kill(pidm, SIGTERM);

	sprintf(menu_title, "%s, Choose A Color:", plr->name);
	char *menu_items[6];
	menu_items[0] = calloc(20, sizeof(char));
	strcpy(menu_items[0], "Red");
	menu_items[1] = calloc(20, sizeof(char));
	strcpy(menu_items[1], "Green");
	menu_items[2] = calloc(20, sizeof(char));
	strcpy(menu_items[2], "Yellow");
	menu_items[3] = calloc(20, sizeof(char));
	strcpy(menu_items[3], "Blue");
	menu_items[4] = calloc(20, sizeof(char));
	strcpy(menu_items[4], "Magenta");
	menu_items[5] = calloc(20, sizeof(char));
	strcpy(menu_items[5], "Cyan");

	int i;
	for(i=0; i<6; i++){
		if((availables[i] == EMPTY) || (availables[i] == INVALID)){
			strcpy(menu_items[i], "TAKEN");}}

	int choice;
	do{
		choice = choice_menu(ui, menu_title, 6, menu_items);}
	while(strcmp(menu_items[choice], "TAKEN") == 0);

	plr->ct = availables[choice];
	availables[choice] = INVALID;
	return;}

void move_piece(Content **b, const int side, Hex from, Hex to) {
	if (get_ct(b, side, from) == EMPTY || get_ct(b, side, from) == INVALID || get_ct(b, side, to) != EMPTY) return;
	set_ct(b, side, to, get_ct(b, side, from)); set_ct(b, side, from, EMPTY);}

void link_accessible_from(UI* ui, Content** b, const int side, int* nb, Hex cells[], int mode){
	if(get_zone(side, cells[*nb-1]) == NOWHERE) return;

	int nb_neighs;
	Hex h = cells[*nb-1]; Hex* neighs = neighbors(side, h, &nb_neighs); Hex tmp_h;
	int i, j, in_cells;
	for(i=0; i<nb_neighs; i++){
		if(get_ct(b, side, neighs[i]) != EMPTY){
			tmp_h.r = h.r + 2*(neighs[i].r - h.r); tmp_h.q = h.q + 2*(neighs[i].q - h.q);
			if(get_ct(b, side, tmp_h) == EMPTY){
				links(ui, side, h, neighs[i], mode); links(ui, side, neighs[i], tmp_h, mode);
				in_cells = 0;
				for(j=0; j<*nb; j++)
					in_cells = in_cells || (tmp_h.r == cells[j].r && tmp_h.q == cells[j].q);
				if(!in_cells){
					cells[(*nb)++] = tmp_h;

					link_accessible_from(ui, b, side, nb, cells, mode);}}}
		else if(get_ct(b, side, h) != EMPTY){

			links(ui, side, h, neighs[i], mode);
			cells[(*nb)++] = neighs[i];}}
	return;}

void play_turn(UI* ui, Content** b, const int side, Player* plr){
	int nb;
	Hex possibilities[side*side];
	int move_made = 0;
	Hex from, to = plr->curs_h;
	while(!move_made && ui->signal != QUIT){
		from = move_cursor(ui, b, side, to);
		if(get_ct(b, side, from) == plr->ct && ui->signal != QUIT){
			nb = 1; possibilities[0] = from;
			link_accessible_from(ui, b, side, &nb, possibilities, 1);
			to = move_cursor(ui, b, side, from);
			nb = 1; possibilities[0] = from;
			link_accessible_from(ui, b, side, &nb, possibilities, 0);
			if((to.r != from.r || to.q != from.q) && in_cell_array(to, nb, possibilities)){
				move_piece(b, side, from, to);
				move_made = 1;}}}
	plr->curs_h = to;
	return;}

int has_won(Content** b, const int side, Player* plr){
	int ret = 1;
	Hex* goal_arr = get_corner_array(side, plr->goal);
	int i = 0;
	while(ret && i<get_corner_size(side)){
		ret = get_ct(b, side, goal_arr[i]) == plr->ct;
		i++;}
	return ret;}
