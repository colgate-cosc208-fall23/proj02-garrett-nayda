/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

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

void easy_AI_instructions(){
    printf("easy AI player instructions: \n\n");
}

void hard_AI_instructions(){
    printf("hard AI player instructions: \n\n");
}
void game_instructions() { // prints game instructions 
    printf("\nwelcome to Battleship! (press CTRL C to quit) \n\n");
    printf("select game mode (press enter/return to continue) \n");
    printf("   * practice mode: press 1\n");
    printf("   * easy AI: press 2\n");
    printf("   * hard AI: press 3\n");
    printf("\n");

    char game_choice;

    do {
        scanf(" %c", &game_choice);
    } 
    while (game_choice != '1' && game_choice != '2' && game_choice !='3');

    printf("\n");
    if (game_choice == '1'){
        practice_mode_instructions();
    } else if (game_choice == '2'){
        easy_AI_instructions();
    } else if (game_choice == '3'){
        hard_AI_instructions();
    }
    
    
}



void print_grid(char **grid) { //takes a grid as parameter --> prints each of the 10 rows
    printf(" 0123456789\n");
    for (int i = 0; i < 10; i++) {
        printf("%d%s\n", i, grid[i]);
    }
}



void free_grid(char **grid) {
    for (int i = 0; i < 10; i++) {
        free(grid[i]);
    }
    free(grid);
}

void free_ships(struct ship *shipArr) {
    free(shipArr);
}

int shipsLeft(struct ship *shipArr) {
    int downCt = 0;
    for (int i = 0; i < 5; i++) {
        if (shipArr[i].shipDown == 1) {
            downCt++;
        }
    }
    return 5 - downCt;
}

int shipDown(struct ship *ship, struct ship *shipArr) {
    if (ship->length == ship->hits) {
        ship->shipDown = 1;
        printf("You sunk the %s! %d ships remaining.\n", ship->name, shipsLeft(shipArr));
        return 1;
    }
    return 0;
}

int gameOver(struct ship *shipArr) {
    int downCt = 0;
    for (int i = 0; i < 5; i++) {
        if (shipArr[i].shipDown == 1) {
            downCt++;
        }
    }
    if (downCt == 5) {
        printf("You win! You have successfully sunk all 5 ships!\n  ");
        return 1;
    }
    return 0;
}

struct ship *create_ships() {
    struct ship *allShips = malloc(5 * sizeof(struct ship));

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

int checkIfGridClear(char **grid, int x, int y, char dir, int len) {
    int lengthAfterStart = len - 1;
    if (grid[x][y] == '-') {
        if (dir == 'l') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((y - i) < 0 || grid[x][y - i] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[x][y - i] = 'O';
            }
        } else if (dir == 'r') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((y + i) > 9 || grid[x][y + i] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[x][y + i] = 'O';
            }
        } else if (dir == 'u') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((x - i) < 0 || grid[x - i][y] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[x - i][y] = 'O';
            }
        } else if (dir == 'd') {
            for (int i = 1; i <= lengthAfterStart; i++) {
                if ((x + i) > 9 || grid[x + i][y] != '-') {
                    return 0;
                }
            }
            for (int i = 0; i < len; i++) {
                grid[x + i][y] = 'O';
            }
        }
        return 1;
    }
    return 0;
}

void fill_ship_coords(struct ship *shipArr, char **grid) {
    char directions[] = { 'u', 'd', 'l', 'r', '\0' }; // creates an array for the possible directions
    srand(time(NULL)); // seed the random number generator

    for (int i = 0; i < 5; i++) { // for each ship
        shipArr[i].hits = 0;
        shipArr[i].shipDown = 0;
        int filled = 0; // checks if ship has been put on grid yet

        while (filled == 0) {
            int x = rand() % 10;
            int y = rand() % 10;
            char direction = directions[rand() % 4]; // random 0-3

            shipArr[i].coords.x = x; // Fills the coords array struct in each ship
            shipArr[i].coords.y = y;
            shipArr[i].direction = direction;

            if (checkIfGridClear(grid, x, y, direction, shipArr[i].length) == 1) {
                filled = 1;
            }
        }
    }
}

void hitWhichShip(int valX, int valY, struct ship *ships) {
    for (int i = 0; i < 5; i++) { // checking each ship
        if (valX == ships[i].coords.x && valY == ships[i].coords.y) { // if it hits the starting coordinates, then automatically done
            ships[i].hits++;
            shipDown(&ships[i], ships);
            break;
        } else if (ships[i].direction == 'u') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valY == ships[i].coords.y && valX == (ships[i].coords.x - j)) {
                    ships[i].hits++;
                    shipDown(&ships[i], ships);
                    break;
                }
            }
        } else if (ships[i].direction == 'd') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valY == ships[i].coords.y && valX == (ships[i].coords.x + j)) {
                    ships[i].hits++;
                    shipDown(&ships[i], ships);
                    break;
                }
            }
        } else if (ships[i].direction == 'l') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valX == ships[i].coords.x && valY == (ships[i].coords.y - j)) {
                    ships[i].hits++;
                    shipDown(&ships[i], ships);
                    break;
                }
            }
        } else if (ships[i].direction == 'r') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valX == ships[i].coords.x && valY == (ships[i].coords.y + j)) {
                    ships[i].hits++;
                    shipDown(&ships[i], ships);
                    break;
                }
            }
        }
    }
}

void fill_player_guess(char **grid, struct ship *ships, char **playerGrid, int *misses,struct ship *shipArr, int *guess_limit) {
    char x[3];
    char y[3];
    int valX;
    int valY;

    printf("enter a row number (0-9): ");
    fgets(x, 3, stdin);
    valX = atoi(x);

    printf("enter a column number (0-9): ");
    fgets(y, 3, stdin);
    valY = atoi(y);

    if ((isdigit(x[0]) == 0 || isdigit(y[0]) == 0) || (valX < 0 || valX > 9) || (valY < 0 || valY > 9)) {
        printf("invalid input! make sure you enter a number 0-9.\n");
        // need to get inputs again. While loop until both are 0-9?
    } else if (grid[valX][valY] == 'M' || grid[valX][valY] == 'H') {
        printf("\ncoordinates already guessed! enter a new set of coordinates.\n");
    } else if (grid[valX][valY] == 'O') {
        grid[valX][valY] = 'H';
        playerGrid[valX][valY] = 'H';
        printf("\nHit at (%d,%d)!  marked with an H (Hit)!\n\n", valX, valY);
        hitWhichShip(valX, valY, ships);
        printf("\nyou have %d ships left to sink.\n\n",shipsLeft(shipArr));
    } else if (grid[valX][valY] == '-') {
        grid[valX][valY] = 'M';
        playerGrid[valX][valY] = 'M';
        printf("\nguess at row %d, column %d was not a hit. marked with an M (Miss).\n", valX, valY);
        (*misses)++;
        printf("\nyou have missed %d attempt(s) so far. you have %d misses remaining. \n", *misses, guess_limit);
        printf("\nyou have %d ships left to sink.\n\n",shipsLeft(shipArr));
        
    }
}

char **grid_maker() { //mallocs and creates a 10x10 grid for graphics and data collection 
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


void play_game() { //highest level function: runs the entirety of the game
    game_instructions();

    printf("press return to continue: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    int guess_limit; //practice mode guess limit - user gets to choose how many tries they'd like to get 
    printf("enter a guess limit: ");
    scanf("%d", &guess_limit);

    char **computer_grid = grid_maker();
    char **player_visible_grid = grid_maker();

    struct ship *all_ships = create_ships();
    fill_ship_coords(all_ships, computer_grid);

    int misses = 0; // Track the number of misses

    while (gameOver(all_ships) == 0) {
        fill_player_guess(computer_grid, all_ships, player_visible_grid, &misses,all_ships, &guess_limit);
        print_grid(player_visible_grid);

        if (misses >= guess_limit) {
            printf("game over! you lose.\n");
            break;
        }
    }



    free_grid(player_visible_grid); //frees all malloced space 
    free_grid(computer_grid);
    free_ships(all_ships);
}


int main() {
    play_game();
}
