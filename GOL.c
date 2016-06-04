/*
* Conway's Game of Life in C.
* Requires ncurses (apt-get install libncurses5-dev)
* gcc -l ncurses GOL.c 
*
*/


#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 0 // Number of times to run (0 means run forever)
#define THROTTLING 30000 //Delay between each iteration
#define INIT_PAUSE 1000000 //Initial delay before first cycle
#define GOSSIPER_GUN_SIZE 36

struct coord{
	int x;
	int y;
};

// Pass a list of coords and a reference to the map object to this function to set initial conditions
int set_inital_coords(struct coord *pattern, int pattern_size, int **map){
	int idx;
	for(idx = 0; idx < pattern_size; idx++){
		map[pattern[idx].x][pattern[idx].y] = 1;
	}

	return 0;
}

// Main loop function: creates/destroys cells based on Conway's rules
int iterate(int **map, int max_x, int max_y){
	int *new_map[max_x];

	int x, y;
	for(x = 0; x < max_x; x++){
		new_map[x] = (int *)malloc(max_y * sizeof(int));

		for(y = 0; y < max_y; y++){

			int live_neighbors = 0;
			live_neighbors += map[x + 1][y];
			live_neighbors += map[x - 1][y];
			live_neighbors += map[x][y + 1];
			live_neighbors += map[x][y - 1];
			live_neighbors += map[x + 1][y + 1];
			live_neighbors += map[x + 1][y - 1];
			live_neighbors += map[x - 1][y + 1];
			live_neighbors += map[x - 1][y - 1];

			if(live_neighbors < 2){ // If cell has less than 2 living neighbors, it dies
				new_map[x][y] = 0;
			}else if(live_neighbors > 3){ // If cell has greater than 3 living neighbors, it dies
				new_map[x][y] = 0;
			}else if(live_neighbors == 3 && !map[x][y]){ // If an empty space has 3 living neighbors, a new cell is created
				new_map[x][y] = 1;
			}else{ // If any space has 2 neighbors, it stays in its last state
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


// Old debugging function to print map to stdout (w/out using ncruces)
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

// Print map object to ncruces
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

// Initialize map randomly
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

// Initialize map w/Gossiper Gun
int gossiper_gun_map(int **map, int max_x, int max_y){
	if(max_x < 40 || max_y < 13){
		return 1;
	}

	struct coord gossiper_gun[GOSSIPER_GUN_SIZE] = {
		{5, 9}, {6, 9}, {5, 10}, {6, 10},
		{17, 7}, {18, 7}, {16, 8}, {15, 9}, {15, 10}, {15, 11}, {16, 12}, {17, 13}, {18, 13},
		{19, 10},
		{20, 8}, {21, 9}, {21, 10}, {21, 11}, {20, 12}, {22, 10},
		{25, 7}, {25, 8}, {25, 9}, {26, 7}, {26, 8}, {26, 9}, {27, 6}, {27, 10},
		{29, 6}, {29, 5}, {29, 10}, {29, 11},
		{39, 7}, {39, 8}, {40, 7}, {40, 8}
	};

	set_inital_coords(gossiper_gun, GOSSIPER_GUN_SIZE, map);

	return 0;
}


int main(int argc, char **argv){
	// Initialize ncurses
	initscr();
	noecho();
	curs_set(FALSE);
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);

	// Initialize map as 2-D array of ints (all 0s at first)
	int *map[max_x + 1];
	int x, y;
	for(x = 0; x < (max_x + 1); x++){
		map[x] = (int *)malloc((max_y + 1) * sizeof(int));
		for(y = 0; y < (max_y + 1); y++){
			map[x][y] = 0;
		}
	}

	// Write initial pattern onto map
	gossiper_gun_map(map, max_x, max_y);
	//random_map(map, max_x, max_y);

	curses_print_map(map, max_x, max_y);
	usleep(INIT_PAUSE);

	// Loop forever
	if(ITERATIONS == 0){
		while(1){
			iterate(map, max_x, max_y);
			curses_print_map(map, max_x, max_y);
		}
	// Loop for a finite number of iterations
	}else{
		int idx;
		for(idx = 0; idx < ITERATIONS; idx++){
			iterate(map, max_x, max_y);
			curses_print_map(map, max_x, max_y);

		}
	}

	// cleanup
	endwin();

	for(x = 0; x < (max_x + 1); x++){
		free(map[x]);
	}

	return 0;
}
