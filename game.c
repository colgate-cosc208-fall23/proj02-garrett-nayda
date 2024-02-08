/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// need to fix valgrind, maybe make a down ship turn into Xs?
#define PRACTICE_MODE 1
#define AI_MODE 2
#define ALL_SHIPS 5

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
    int ship_sunk;
    char name[15];
    struct coord coords;
};

void practice_mode_instructions(){
    printf("Practice mode instructions: \n\n");
    sleep(1);
    printf("   * 5 ships will be randomly placed on the grid: the Carrier (5), the Battleship (4), the Cruiser (3), the Submarine (3), and the Destroyer (2).\n");
    sleep(2);
    printf("   * Each turn, you will prompted to input a row and column (0-9)...\n");
    sleep(2);
    printf("   * If you hit a ship, it will be marked with an 'H' for hit.\n");
    sleep(2);
    printf("   * A missed guess will be marked with an 'M' for miss.\n");
    sleep(2);
    printf("   * If you hit all of the ships within your guess limit, you win. Otherwise, you lose.\n\n");
    sleep(1);
}

void AI_instructions(){
    printf("AI mode instructions: \n\n");
    sleep(1);
    printf("   * You will be prompted to place 5 ships on the grid: the Carrier (5), the Battleship (4), the Cruiser (3), the Submarine (3), and the Destroyer (2).\n");
    sleep(2);
    printf("   * Computer ships will be randomly placed\n");
    sleep(2);
    printf("   * You and the computer will take turns guessing where each others ships are\n");
    sleep(2);
    printf("   * Each turn, you will prompted to input a row and column (0-9)...\n");
    sleep(2);
    printf("   * If you hit a ship, it will be marked with an 'H' for hit.\n");
    sleep(2);
    printf("   * A missed guess will be marked with an 'M' for miss.\n");
    sleep(2);
    printf("   * If you hit all of the ships before the AI hits yours, you win. Otherwise, you lose.\n\n");
    sleep(1);


    
}

int game_instructions() { // prints game instructions 
    printf("\nWelcome to Battleship! (CTRL C to quit) \n\n");
    sleep(1);
    printf("Select game mode (press enter/return to continue) \n");
    sleep(1);
    printf("   * Practice mode: press 1\n");
    sleep(1);
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
        if (ship_arr[i].ship_sunk == 1) {
            downCt++;
        }
    }
    return 5 - downCt;
}

int ship_down(struct ship *ship, struct ship *ship_arr) {
    if (ship->length == ship->hits) {
        ship->ship_sunk= 1;
        printf("You sunk the %s! %d ships remaining.\n", ship->name, ships_left(ship_arr));
        return 1;
    }
    return 0;
}

int all_ships_down(struct ship *ship_arr) {
    for (int i = 0; i < 5; i++) {
        if (ship_arr[i].ship_sunk == 0) {
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

int place_ship(char **grid, int row, int col, char dir, int len) {
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
        } else {
            return 0; // if an invalid direction was given 
        }

        return 1;
    }
    return 0;
}

void random_ship_placement(struct ship *ship_arr, char **grid) {
    char directions[] = { 'u', 'd', 'l', 'r', '\0' }; // creates an array for the possible directions
    srand(time(NULL)); // seed the random number generator

    for (int i = 0; i < 5; i++) { // for each ship
        ship_arr[i].hits = 0;
        ship_arr[i].ship_sunk = 0; // false --> ship is not down
        int filled = 0; // checks for potential collisions 

        while (filled == 0) {
            int x = rand() % 10; //generate a random x and y
            int y = rand() % 10;
            char direction = directions[rand() % 4]; // choose a random direction

            ship_arr[i].coords.x = x; 
            ship_arr[i].coords.y = y;
            ship_arr[i].direction = direction;

            if (place_ship(grid, x, y, direction, ship_arr[i].length) == 1) {
                filled = 1;
            }
        }
    }
}
    

void player_ship_placement(struct ship* ship_arr, char **grid){
    printf("places player ships\n");

    for (int i = 0; i < ALL_SHIPS;  i++){
        int row; //establish all necessary variables 
        int col;
        char direction;
        int len = ship_arr[i].length;
        int done = 0;

        ship_arr[i].hits = 0;
        ship_arr[i].ship_sunk = 0; 

        printf("\nFilling %s (length %d): \n", ship_arr[i].name, len);
        sleep(1);
        while (done == 0) {
            printf("Enter a starting row number (0-9): "); // now get user input on the starting row, column, and direction
            scanf("%d", &row);

            printf("Enter a starting col number (0-9): ");
            scanf("%d", &col);
            getchar();

            printf("Enter a direction -> 'u' (up), 'd' (down), 'l' (left), or 'r' (right): ");
            scanf("%c", &direction);
            sleep(1);
        

            ship_arr[i].coords.x = row; 
            ship_arr[i].coords.y = col;
            ship_arr[i].direction = direction;

            if (place_ship(grid, row, col, direction, len) == 1) {
                done = 1;
                printf("\n%s successfully placed.\n", ship_arr[i].name);
                sleep(1);
                printf("\nPlayer grid: \n");
                print_grid(grid);
            } else {
                sleep(1);
                printf("Invalid input/ship collision. Try again. \n\n");
            }            
        }
    }
    
    printf("All ships placed!\nTime to play!\n\n");
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

char fill_player_guess(char **grid, struct ship *ships, char **playerGrid, struct ship *ship_arr) {
    int row;
    int col;
    int valid = 0;

    while (valid == 0){
        printf("Enter a row number (0-9): ");
        scanf("%d", &row);

        printf("Enter a column number (0-9): ");
        scanf("%d", &col);


        if ((row < 0 || row > 9) || (col < 0 || col > 9)) {
            printf("\ninvalid input - enter a number 0-9.\n");
        } else if (grid[row][col] != '-' && grid[row][col] != 'O') {
            printf("\ncoordinates already guessed - enter a new set of coordinates.\n");
        } else {
            valid = 1;
        }
    } 

    if (grid[row][col] == 'O') {
        grid[row][col] = 'H';
        playerGrid[row][col] = 'H';
        sleep(1);
        printf("\nGuess at row %d, column %d was a...", row, col);
         // wanted to put sleep here but doesn't seem to work
        printf(" hit! marked with an H (hit)\n\n");
        hit_which_ship(row, col, ships);
        printf("You have %d ships left to sink.\n\n", ships_left(ship_arr));
        return 'H';
    } else if (grid[row][col] == '-') {
        grid[row][col] = 'M';
        playerGrid[row][col] = 'M';
        sleep(1);
        printf("\nGuess at row %d, column %d was a...", row, col);
        // wanted to put sleep here but doesn't seem to work
        printf(" miss! marked with an M (miss)\n\n");
        return 'M';
    } else {
        return 'F';
    }
}

void computer_guess(char **player_grid, struct ship *player_ships){ //given a player grid, the computer makes a random guess 
    int done = 0; // gonna do a while loop until I'm done

    while (done == 0){ //keep guessing
        int row = rand() % 10; //generate a random x and y in bounds
        int col = rand() % 10;

        printf("computer guess: row %d, col %d\n", row, col);

        if (player_grid[row][col] == '-'){
            printf("AI missed\n");
            player_grid[row][col] = 'M';
            done = 1;
        }
        else if (player_grid[row][col] == 'O'){
            printf("Your ship has been hit\n");
            player_grid[row][col] = 'H';
            hit_which_ship(row, col, player_ships);
            printf("grid\n");
            print_grid(player_grid);
            done = 1;
        }
            
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
    char hit_or_miss;

    sleep(1);
    printf("\nEnter a guess limit: ");
    scanf("%d", &guess_limit);

    char **computer_grid = grid_maker(); // two grids - one tracks the data and another displays hits to the users 
    char **player_visible_grid = grid_maker();

    struct ship *all_ships = create_ships();
    random_ship_placement(all_ships, computer_grid); //look into this function more --- it seems pretty messy 

    printf("\nTime to play!\n\n");
     while (all_ships_down(all_ships) == 0 && guess_limit != misses) {
        sleep(1);
        hit_or_miss = fill_player_guess(computer_grid, all_ships, player_visible_grid, all_ships);
        printf("\n");
        sleep(1);
        print_grid(player_visible_grid);
        printf("\n");
        if (hit_or_miss == 'M'){
            misses++;
            printf("%d guess(es) remain.\n\n", guess_limit-misses);
        }
     }

     if (guess_limit == misses){
        printf("You lose! Game over.\n\nEnemy ships: \n\n");
        print_grid(computer_grid);
     } else {
        printf("\nYou have successfully sunk all enemy ships! You win.");
     }

    
    free_grid(player_visible_grid); //frees all malloced space 
    free_grid(computer_grid);
    free_ships(all_ships);
}


void AI_game(){
    char **player_grid = grid_maker(); //player's personal grid

    char **AI_grid = grid_maker(); // computer's grid
    char **AI_visible_grid = grid_maker(); // version of computer grid that player is allowed to see


    struct ship *player_ships = create_ships();
    struct ship *AI_ships = create_ships();

    player_ship_placement(player_ships, player_grid);
    random_ship_placement(AI_ships, AI_grid); //randomly fill the computer's grid
 
   
    while (all_ships_down(player_ships) == 0 && all_ships_down(AI_ships) == 0){
        fill_player_guess(AI_grid, AI_ships, AI_visible_grid, AI_ships);
        print_grid(AI_visible_grid);
        sleep(3);
        computer_guess(player_grid, player_ships);
        print_grid(player_grid);

        //   // player guess 
    //computer guess
    }

    // player guess
    //computer guess 

    // guess - same as in practice mode

    // then have the ai guess one
    // must be within bounds, can not guess the same coordinates twice

    // each time it checks if the ships are down. that's all that it checks.
    // if ships are down, other person wins 
}


void play_game() { //highest level function: runs the entirety of the game
    int mode = game_instructions();
    printf("Press enter/return to continue "); 

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
