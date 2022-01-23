#ifndef DATA_STRUCT_H_INCLUDED
#define DATA_STRUCT_H_INCLUDED
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

extern char gameover[];
extern char cursormove[];
extern char next[];
extern char menu[];

typedef struct{
	int i;
	int j;
} Stor;
Stor new_stor(int, int);
typedef struct{
	int r;
	int q;
} Hex;
Hex new_hex(int, int);
typedef struct{
	int x;
	int y;
	int z;
} Cube;
Cube new_cube(int, int, int);
typedef enum content { EMPTY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, INVALID } Content;
typedef enum zone { CENTER, TOP, TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT, BOT, NOWHERE } Zone;
Stor hex_to_stor(const int, Hex);
Hex stor_to_hex(const int, Stor);
Cube hex_to_cube(Hex);
Hex cube_to_hex(Cube);
Content** init_board(const int);
int boardh(const int);
int linew(const int, int);
Zone get_zone(const int, Hex);
Zone get_opposite(Zone);
int get_corner_size(const int);
Hex* get_corner_array(const int, Zone);
void set_corner_ct(Content**, const int, Zone, Content);
Content get_ct(Content**, const int, Hex);
void set_ct(Content**, const int, Hex, Content);
int distance(Hex, Hex);
Hex* neighbors(const int, Hex, int*);
int in_cell_array(Hex, int, Hex*);
int sound(char *name, pid_t pid);
#endif
