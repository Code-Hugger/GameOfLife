/*
* Conway's Game of Life in C.
* Requires ncurses (apt-get install libncurses5-dev)
* gcc -l ncurses GOL.c 
*
*
*/


#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 0 // Number of times to run (0 means run forever)
#define THROTTLING 30000 //Delay between each iteration


int iterate(int **map, int max_x, int max_y){
	int *new_map[max_x];

	int x, y;
	for(x = 0; x < max_x; x++){
		new_map[x] = (int *)malloc(max_y * sizeof(int));

		for(y = 0; y < max_y; y++){
			new_map[x][y] = 0;

			int live_neighbors = 0;
			live_neighbors += map[x + 1][y];
			live_neighbors += map[x - 1][y];
			live_neighbors += map[x][y + 1];
			live_neighbors += map[x][y - 1];
			live_neighbors += map[x + 1][y + 1];
			live_neighbors += map[x + 1][y - 1];
			live_neighbors += map[x - 1][y + 1];
			live_neighbors += map[x - 1][y - 1];

			if(live_neighbors < 2){
				new_map[x][y] = 0;
			}else if(live_neighbors > 3){
				new_map[x][y] = 0;
			}else if(live_neighbors == 3 && !map[x][y]){
				new_map[x][y] = 1;
			}else{
				new_map[x][y] = map[x][y];
			}

		}
	} 

	for(x = 0; x < max_x; x++){
		for(y = 0; y < max_y; y++){
			map[x][y] = new_map[x][y];
		}

		free(new_map[x]);
	}


	return 0;
}


int print_map(int **map, int max_x, int max_y){
	int x;

	for(x = 0; x < max_x + 1; x++){
		int y;
		for(y = 0; y < max_y + 1; y++){
			
			if(map[x][y]){
				printf("*");
			}else{
				printf(" ");
			}
		}

		printf("\n");
	}
	return 0;
}

int curses_print_map(int **map, int max_x, int max_y){
	clear();

	int x, y;
	for(x = 0; x < max_x; x++){
		for(y = 0; y < max_y; y++){
			if(map[x][y]){
				mvprintw(y, x, "*");
			}
			
		}
	}

	refresh();
	usleep(THROTTLING);

	return 0;
}


int random_map(int **map, int max_x, int max_y){
	srand(time(NULL));

	int x, y;
	for(x = 0; x < max_x; x++){
		for(y = 0; y < max_y; y++){
			int r = rand() % 2;
			map[x][y] = r;
		}
	}
	return 0;
}

int gossiper_gun_map(int **map, int max_x, int max_y){
	// Simple gossiper gun

	if(max_x < 40 || max_y < 13){
		return 1;
	}

	map[5][9] = 1;
	map[6][9] = 1;
	map[5][10] = 1;
	map[6][10] = 1;

	map[17][7] = 1;
	map[18][7] = 1;
	map[16][8] = 1;
	map[15][9] = 1;
	map[15][10] = 1;
	map[15][11] = 1;
	map[16][12] = 1;
	map[17][13] = 1;
	map[18][13] = 1;
	
	map[19][10] = 1;

	map[20][8] = 1;
	map[21][9] = 1;
	map[21][10] = 1;
	map[21][11] = 1;
	map[20][12] = 1;
	map[22][10] = 1;

	map[25][7] = 1;
	map[25][8] = 1;
	map[25][9] = 1;
	map[26][7] = 1;
	map[26][8] = 1;
	map[26][9] = 1;
	map[27][6] = 1;
	map[27][10] = 1;

	map[29][6] = 1;
	map[29][5] = 1;
	map[29][10] = 1;
	map[29][11] = 1;

	map[39][7] = 1;
	map[39][8] = 1;
	map[40][7] = 1;
	map[40][8] = 1;

	return 0;
}

int main(int argc, char **argv){
	// Initialize ncurses
	initscr();
	noecho();
	curs_set(FALSE);
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);

	int *map[max_x + 1];
	int x, y;
	for(x = 0; x < (max_x + 1); x++){
		map[x] = (int *)malloc((max_y + 1) * sizeof(int));
		for(y = 0; y < (max_y + 1); y++){
			map[x][y] = 0;
		}
	}


	gossiper_gun_map(map, max_x, max_y);
	//random_map(map, max_x, max_y);
	curses_print_map(map, max_x, max_y);
	usleep(1000000);




	if(ITERATIONS == 0){
		while(1){
			iterate(map, max_x, max_y);
			curses_print_map(map, max_x, max_y);
		}
	}else{
		int idx;
		for(idx = 0; idx < ITERATIONS; idx++){
			iterate(map, max_x, max_y);
			curses_print_map(map, max_x, max_y);

		}
	}

	// End ncurses window
	endwin();

	for(x = 0; x < (max_x + 1); x++){
		free(map[x]);
	}

	return 0;
}
