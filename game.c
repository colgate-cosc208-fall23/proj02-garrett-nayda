/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define PRACTICE_MODE 1
#define AI_MODE 2

struct coord { // struct that represents coordinate on the grid
    int x;
    int y;
};

struct ship { //struct with all ship information
    int startingX; // must be 0-9
    int startingY; // must be 0-9
    char direction; // u, d, l, or r
    int length; // length of the ship, note that we already have the first coord
    int hits;
    int shipDown;
    char name[15];
    struct coord coords;
};

void practice_mode_instructions(){
    printf("practice mode player instructions: \n\n");
    printf("   * 5 ships will be randomly placed on the grid: the Carrier (5), the Battleship (4), the Cruiser (3), the Submarine (3), and the Destroyer (2).\n");
    printf("   * each turn, you will prompted to input a set of coordinates (0,0) <= (x, y) <= (9,9)...\n");
    printf("   * if you hit a ship, it will be marked with an 'H' for hit.\n");
    printf("   * a missed guess will be marked with an 'M' for miss.\n");
    printf("   * if you hit all of the ships within your guess limit, you win. otherwise, you lose.\n\n");
}

void AI_instructions(){
    printf("easy AI player instructions: \n\n");
    printf("not implemented yet :'(\n");
}

int game_instructions() { // prints game instructions 
    printf("\nwelcome to Battleship! (press CTRL C to quit) \n\n");
    printf("select game mode (press enter/return to continue) \n");
    printf("   * practice mode: press 1\n");
    printf("   * AI: press 2\n\n");

    char game_choice;
    do {
        scanf(" %c", &game_choice);
    } while (game_choice != '1' && game_choice != '2');

    printf("\n");
    if (game_choice == '1'){
        practice_mode_instructions();
        return 1;
    } else if (game_choice == '2'){
        AI_instructions();
        return 2;
    } 

    return -1;  
}



void print_grid(char **grid) { 
    // given a grid, print_grid prints the grid
    printf(" 0123456789\n");
    for (int i = 0; i < 10; i++) {
        printf("%d%s\n", i, grid[i]);
    }
}



void free_grid(char **grid) {
    // given a grid, free_grid frees all malloced space for a grid
    for (int i = 0; i < 10; i++) {
        free(grid[i]);
    }
    free(grid);
}

void free_ships(struct ship *ship_arr) {
    //given an array of ships, free_ships frees all malloced space used for ships
    free(ship_arr);
}

int ships_left(struct ship *ship_arr) {
    //given an array of ships, ship_left returns an int corresponding to the number of ships that have not been sunk. 
    int downCt = 0;
    for (int i = 0; i < 5; i++) {
        if (ship_arr[i].shipDown == 1) {
            downCt++;
        }
    }
    return 5 - downCt;
}

int ship_down(struct ship *ship, struct ship *ship_arr) {
    if (ship->length == ship->hits) {
        ship->shipDown = 1;
        printf("You sunk the %s! %d ships remaining.\n", ship->name, ships_left(ship_arr));
        return 1;
    }
    return 0;
}

int all_ships_down(struct ship *ship_arr) {
    for (int i = 0; i < 5; i++) {
        if (ship_arr[i].shipDown == 0) {
            return 0; //false 
        }
    }
    return 1;
}

struct ship *create_ships() { //creates an array of all 5 ships 

    struct ship *allShips = malloc(5 * sizeof(struct ship)); //array of 5 ships 

    struct ship *patrolBoat = &allShips[0];
    patrolBoat->length = 2;
    strcpy(patrolBoat->name, "Patrol Boat");

    struct ship *submarine = &allShips[1];
    submarine->length = 3;
    strcpy(submarine->name, "Submarine");

    struct ship *destroyer = &allShips[2];
    destroyer->length = 3;
    strcpy(destroyer->name, "Destroyer");

    struct ship *battleship = &allShips[3];
    battleship->length = 4;
    strcpy(battleship->name, "Battleship");

    struct ship *carrier = &allShips[4];
    carrier->length = 5;
    strcpy(carrier->name, "Carrier");

    return allShips;
}

int check_if_grid_clear(char **grid, int row, int col, char dir, int len) {
    int lengthAfterStart = len - 1;
    if (grid[row][col] == '-') {
        if (dir == 'l') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((col - i) < 0 || grid[row][col - i] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[row][col - i] = 'O';
            }
        } else if (dir == 'r') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((col + i) > 9 || grid[row][col + i] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[row][col + i] = 'O';
            }
        } else if (dir == 'u') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((row - i) < 0 || grid[row - i][col] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[row - i][col] = 'O';
            }
        } else if (dir == 'd') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((row + i) > 9 || grid[row + i][col] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[row + i][col] = 'O';
            }
        }
        return 1;
    }
    return 0;
}

void fill_ship_coords(struct ship *ship_arr, char **grid) {
    char directions[] = { 'u', 'd', 'l', 'r', '\0' }; // creates an array for the possible directions
    srand(time(NULL)); // seed the random number generator

    for (int i = 0; i < 5; i++) { // for each ship
        ship_arr[i].hits = 0;
        ship_arr[i].shipDown = 0; // false --> ship is not down
        int filled = 0; // checks for potential collisions 

        while (filled == 0) {
            int x = rand() % 10; //generate a random x and y
            int y = rand() % 10;
            char direction = directions[rand() % 4]; // choose a random direction

            ship_arr[i].coords.x = x; 
            ship_arr[i].coords.y = y;
            ship_arr[i].direction = direction;

            if (check_if_grid_clear(grid, x, y, direction, ship_arr[i].length) == 1) {
                filled = 1;
            }
        }
    }
}

void hit_which_ship(int x, int y, struct ship *ships) {
    for (int i = 0; i < 5; i++) { // checking each ship
        if (x == ships[i].coords.x && y == ships[i].coords.y) { // if it hits the starting coordinates, then automatically done
            ships[i].hits++;
            ship_down(&ships[i], ships);
            break;
        } else if (ships[i].direction == 'u') {
            for (int j = 1; j < ships[i].length; j++) {
                if (x == ships[i].coords.y && y == (ships[i].coords.x - j)) {
                    ships[i].hits++;
                    ship_down(&ships[i], ships);
                    break;
                }
            }
        } else if (ships[i].direction == 'd') {
            for (int j = 1; j < ships[i].length; j++) {
                if (y == ships[i].coords.y && x == (ships[i].coords.x + j)) {
                    ships[i].hits++;
                    ship_down(&ships[i], ships);
                    break;
                }
            }
        } else if (ships[i].direction == 'l') {
            for (int j = 1; j < ships[i].length; j++) {
                if (x == ships[i].coords.x && y == (ships[i].coords.y - j)) {
                    ships[i].hits++;
                    ship_down(&ships[i], ships);
                    break;
                }
            }
        } else if (ships[i].direction == 'r') {
            for (int j = 1; j < ships[i].length; j++) {
                if (x == ships[i].coords.x && y == (ships[i].coords.y + j)) {
                    ships[i].hits++;
                    ship_down(&ships[i], ships);
                    break;
                }
            }
        }
    }
}

void fill_player_guess(char **grid, struct ship *ships, char **playerGrid, int *misses,struct ship *ship_arr, int *guess_limit) {
    int x;
    int y;

    printf("Enter a row number (0-9): ");
    scanf("%d", &x);

    printf("Enter a column number (0-9): ");
    scanf("%d", &y);

    if ((x < 0 || x > 9) || (y < 0 || y > 9)) {
        printf("\ninvalid input - enter a number 0-9.\n");
    } else if (grid[x][y] != '-') {
        printf("\ncoordinates already guessed - enter a new set of coordinates.\n");
    } else if (grid[x][y] == 'O') {
        grid[x][y] = 'H';
        playerGrid[x][y] = 'H';
        printf("\nhit at (%d,%d)! marked with an H (Hit)!\n\n", x, y);
        hit_which_ship(x, y, ships);
        printf("\nyou have %d ships left to sink.\n\n", ships_left(ship_arr));
    } else if (grid[x][y] == '-') {
        grid[x][y] = 'M';
        playerGrid[x][y] = 'M';
        printf("\nguess at row %d, column %d was not a hit. marked with an M (miss).\n", x, y);
        (*misses)++;
        printf("\nyou have missed %d attempt(s) so far. you have %d misses remaining.\n", *misses, (*guess_limit - *misses));
        printf("\nyou have %d ships left to sink.\n\n", ships_left(ship_arr));
    }
}

char **grid_maker() { //mallocs and fills a 10x10 grid for graphics and data collection 
    char **grid = malloc(sizeof(char *) * 10); //malloc space for a 10 x 10 2D array
    for (int i = 0; i < 10; i++) { 
        grid[i] = malloc(sizeof(char) * 11); //create row
        for (int j = 0; j < 10; j++) {
            grid[i][j] = '-'; //fill with dashes
        } 
        grid[i][10] = '\0'; //null terminate last index of each inner array 
    }

    return grid;
}

void practice_game(){
    int misses = 0; // tracks the number of misses  
    int guess_limit; //practice mode guess limit - user gets to choose how many tries they'd like to get 
    printf("\nenter a guess limit: ");
    scanf("%d", &guess_limit);

    char **computer_grid = grid_maker(); // two grids - one tracks the data and another displays hits to the users 
    char **player_visible_grid = grid_maker();

    struct ship *all_ships = create_ships();
    fill_ship_coords(all_ships, computer_grid); //look into this function more --- it seems pretty messy 

     while (all_ships_down(all_ships) == 0 && guess_limit != misses) {
        fill_player_guess(computer_grid, all_ships, player_visible_grid, &misses, all_ships, &guess_limit);
        print_grid(player_visible_grid);
     }
     if (guess_limit == misses){
        printf("\nNo guesses remain! Game over.");
     }
     else {
        printf("\nYou have successfully sunk all enemy ships! You win.");
     }

    
    free_grid(player_visible_grid); //frees all malloced space 
    free_grid(computer_grid);
    free_ships(all_ships);
}

void AI_game(){
    printf("this is AI mode\n");

    char **player_grid = grid_maker(); //player's personal grid
    char **player_visible_grid = grid_maker(); // version of computer grid that player is allowed to see
    char **computer_grid = grid_maker(); // computer's grid

    struct ship *player_ships = create_ships();
    struct  ship *enemy_ships = create_ships();

    //create the AI grid
    //use the random generator to fill in the AI grid

    // let player start first

    // guess - same as in practice mode

    // then have the ai guess one
    // must be within bounds, can not guess the same coordinates twice

    // each time it checks if the ships are down. that's all that it checks.
    // if ships are down, other person wins 
}


void play_game() { //highest level function: runs the entirety of the game
    int mode = game_instructions();
    printf("press return to continue "); 

    int c; 
    while ((c = getchar()) != '\n');
    getchar();

    if (mode == PRACTICE_MODE){ 
       practice_game();
    }

    if (mode == AI_MODE){
        AI_game();
    } 
   
}

int main() {
    play_game();
}
