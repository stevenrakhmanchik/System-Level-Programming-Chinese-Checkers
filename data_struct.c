#include "data_struct.h"
#include <string.h>
#include <stdio.h>
Stor new_stor(int i, int j){
	Stor s;
	s.i = i; s.j = j;
	return s;}

Hex new_hex(int r, int q){
	Hex h;
	h.r = r; h.q = q;
	return h;}

Cube new_cube(int x, int y, int z){
	Cube c;
	c.x = x; c.y = y; c.z = z;
	return c;}

Stor hex_to_stor(const int side, Hex h){
	Stor s;
	s.i = h.r + (2*side-2);
	int sjq0;
	if(s.i<side-1 || (s.i>=2*side-1 && s.i<3*side-2)){
		sjq0 = linew(side, s.i)-side;}
		else{
		sjq0 = side-1;}
	s.j = h.q + sjq0;
	return s;}

Hex stor_to_hex(const int side, Stor s){
	Hex h;
	h.r = s.i - (2*side-2);
	int sjq0;
	if(s.i<side-1 || (s.i>=2*side-1 && s.i<3*side-2)){
		sjq0 = linew(side, s.i)-side;}
		else{
		sjq0 = side-1;}
	h.q = s.j - sjq0;
	return h;}

Cube hex_to_cube(Hex h){
	Cube c;
	c.z = h.r; c.x = h.q; c.y = 0 - c.x - c.z;
	return c;}

Hex cube_to_hex(Cube c){
	Hex h;
	h.r = c.z; h.q = c.x;
	return h;}

Content** init_board(const int side){
	int total_nb = 6*side*side-4*side+1;
	Content** b = (Content**) malloc(sizeof(Content)*total_nb);
	int i, j;
	for(i=0; i<boardh(side); i++){
		b[i] = (Content*) malloc(sizeof(Content)*linew(side, i));
		for(j=0; j<linew(side, i); j++){
			if(get_zone(side, stor_to_hex(side, new_stor(i, j))) == CENTER){
				b[i][j] = EMPTY;}
				else{
				b[i][j] = INVALID;}} }
	return b;}

int boardh(const int side){
	return 4*side-3;}

int linew(const int side, int row){
	int width = 0;
	if(row<side-1 || (row>=2*side-1 && row<3*side-2)){
		width = row+1;}
		else{
		width = 4*side-3-row;}
	return width;}

Zone get_zone(const int side, Hex h){
	Stor s = hex_to_stor(side, h);
	Cube c = hex_to_cube(h);
	if(s.i < 0 || s.i >= boardh(side) || s.j < 0 || s.j >= linew(side, s.i)){
		return NOWHERE;}
	else if(c.z < 1-side){
		return TOP;}
	else if(c.z > side-1){
		return BOT;}
	else if(c.x > side-1){
		return TOP_RIGHT;}
	else if(c.x < 1-side){
		return BOT_LEFT;}
	else if(c.y > side-1){
		return TOP_LEFT;}
	else if(c.y < 1-side){
		return BOT_RIGHT;}
	else{
		return CENTER;}}

Zone get_opposite(Zone zn){
	Zone ret;
	switch(zn){
	case TOP:
		ret = BOT; break;
	case TOP_LEFT:
		ret = BOT_RIGHT; break;
	case BOT_LEFT:
		ret = TOP_RIGHT; break;
	case BOT:
		ret = TOP; break;
	case BOT_RIGHT:
		ret = TOP_LEFT; break;
	case TOP_RIGHT:
		ret = BOT_LEFT; break;
	default:
		ret = NOWHERE; break;}
	return ret;}

int get_corner_size(const int side){
	return (side*side-side)/2;}

Hex* get_corner_array(const int side, Zone corner){
	if(corner == CENTER || corner == NOWHERE) return NULL;
	Hex cur; Hex to_next_line; Hex nextInLine; Hex* hexes = (Hex*) malloc(get_corner_size(side)*sizeof(Hex));
	switch(corner){
	case TOP:
		cur = new_hex(-2*(side-1), side-1);
		to_next_line = new_hex(1, -1);
		nextInLine = new_hex(0, 1);
		break;
	case TOP_LEFT:
		cur = new_hex(1-side, 1-side);
		to_next_line = new_hex(1, 0);
		nextInLine = new_hex(-1, 1);
		break;
	case BOT_LEFT:
		cur = new_hex(side-1, -2*(side-1));
		to_next_line = new_hex(0, 1);
		nextInLine = new_hex(-1, 0);
		break;
	case BOT:
		cur = new_hex(2*(side-1), 1-side);
		to_next_line = new_hex(-1, 1);
		nextInLine = new_hex(0, -1);
		break;
	case BOT_RIGHT:
		cur = new_hex(side-1, side-1);
		to_next_line = new_hex(-1, 0);
		nextInLine = new_hex(1, -1);
		break;
	case TOP_RIGHT:
		cur = new_hex(1-side, 2*(side-1));
		to_next_line = new_hex(0, -1);
		nextInLine = new_hex(1, 0);
		break;
	default:
	//THIS SHOULD NOT HAPPEN
		break;}
	hexes[0] = cur;
	int line_len = 1; int pos_in_line = 1; int i;
	for(i=1; i<get_corner_size(side); i++){
		if(pos_in_line >= line_len){
			cur.r -= nextInLine.r*(line_len-1); cur.q -= nextInLine.q*(line_len-1);
			cur.r += to_next_line.r; cur.q += to_next_line.q;
			line_len++;
			pos_in_line = 1;}
		else{
			cur.r += nextInLine.r; cur.q += nextInLine.q;
			pos_in_line++;}
		hexes[i] = cur;}
	return hexes;}

void set_corner_ct(Content** b, const int side, Zone zn, Content ct){
	if(zn == CENTER || zn == NOWHERE) return;
	Hex *hexes = get_corner_array(side, zn);
	int i;
	for(i=0; i<get_corner_size(side); i++){set_ct(b, side, hexes[i], ct);}
	free(hexes);
	return;}

Content get_ct(Content** b, const int side, Hex h){
	if(get_zone(side, h) == NOWHERE) return INVALID;
	Stor s = hex_to_stor(side, h);
	return b[s.i][s.j];}

void set_ct(Content** b, const int side, Hex h, Content c){
	Stor s = hex_to_stor(side, h);
	b[s.i][s.j] = c;
	return;}

int distance(Hex h1, Hex h2){
	Cube c1 = hex_to_cube(h1); Cube c2 = hex_to_cube(h2);
	return (abs(c1.x-c2.x)+abs(c1.y-c2.y)+abs(c1.z-c2.z))/2;}

Hex* neighbors(const int side, Hex h, int* len){
	int i = 0;
	Hex tmp_h; Hex* neighs = (Hex*) malloc(6*sizeof(Hex));

	//STRAIGHT RIGHT
	tmp_h = new_hex(h.r, h.q+1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;
	//UP RIGHT
	tmp_h = new_hex(h.r-1, h.q+1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;
	//UP LEFT
	tmp_h = new_hex(h.r-1, h.q);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;
	//STRAIGHT LEFT
	tmp_h = new_hex(h.r, h.q-1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;
	//DOWN LEFT
	tmp_h = new_hex(h.r+1, h.q-1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;
	//DOWN RIGHT
	tmp_h = new_hex(h.r+1, h.q);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;
	*len = i;
	neighs = realloc(neighs, sizeof(Hex)*i);
	return neighs;}

int in_cell_array(Hex h, int len, Hex* arr){
	int i = 0; int ret = 0;
	while(i<len && !ret){
		ret = (h.r == arr[i].r) && (h.q == arr[i].q);
		i++;}
	return ret;}

	int sound(char *name, pid_t pid){
			system(name);}	//	MacOS command for playing sounds via CLI}
		//kill(pid, SIGSTOP);
