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

#define PRACTICE_MODE 1
#define AI_MODE 2
#define ALL_SHIPS 5
#define ERR -1

void clear_input_buffer() { //clears input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void check_new_line(char* input) { // checks if there's a new line char in terminal (fixes bug so that you don't have to press enter twice)
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    } else {
        clear_input_buffer(); // clear input buffer if newline is not present
    }
}

struct guess { // struct that represents coordinate on the grid
    int row;
    int col;
};

struct ship { //struct with all ship information
    char name[15];
    struct guess start; //starting row and col
    char direction; 
    int length; 
    int hits;
    int ship_sunk; // int representing whether a ship is sunk: 0 false, 1 true
};

void practice_mode_instructions(){ // prints instructions for practice game mode
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

void AI_instructions(){ //prints instructions for AI game mode
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

    char input[10]; 
    int game_choice;
    
    while (1) { //input validation
        printf("Enter your choice (1 or 2): ");
        fgets(input, sizeof(input), stdin);
        
        check_new_line(input);

        if (strlen(input) != 1 || (input[0] != '1' && input[0] != '2')) {
            printf("Invalid input. Must be 1 or 2. Try again.\n\n");
            continue;
        }
        game_choice = input[0] - '0'; 
        break;
    }
    printf("\n");

    if (game_choice == PRACTICE_MODE){ // determines which mode 
        practice_mode_instructions();
        return PRACTICE_MODE;
    } else if (game_choice == AI_MODE){
        AI_instructions();
        return AI_MODE;
    } 
    return ERR;  
}

void print_grid(char **grid) {   // given a 2D char array respresenting a grid, print_grid prints the grid 
    printf("\n");
    printf(" 0123456789\n");
    for (int i = 0; i < 10; i++) {
        printf("%d%s\n", i, grid[i]);
    }
    printf("\n");
}

void free_grid(char **grid) { // frees all malloced space for a grid
    for (int i = 0; i < 10; i++) {
        free(grid[i]);
    }
    free(grid);
}

void free_ships(struct ship *ship_arr) {  // frees all malloced space used for ships
    free(ship_arr);
}

int ships_left(struct ship *ship_arr) { // returns an int corresponding to the number of ships that have not been sunk. 
    int downCt = 0;
    for (int i = 0; i < 5; i++) {
        if (ship_arr[i].ship_sunk == 1) {
            downCt++;
        }
    }
    return 5 - downCt;
}

void ship_down_grid(struct ship *ship, char **grid){ // changes the grid to represent a sunk ship (all Xs)
    for (int i = 0; i < ship->length; i++){
        if (ship->direction == 'H' || ship->direction == 'h' ){
            grid[ship->start.row][ship->start.col+i] = 'X';
        } else if (ship-> direction == 'V' || ship-> direction =='v' ){
            grid[ship->start.row + i][ship->start.col] = 'X';
        }
    }
}

int ship_down_check(struct ship *ship, struct ship *ship_arr, char **grid) {  // returns an int representing whether a given ship has been sunk
    if (ship->length == ship->hits) {
        ship->ship_sunk= 1;
        printf("%s sunk! %d ships remaining.\n", ship->name, ships_left(ship_arr));
        ship_down_grid(ship, grid);
        return 1;
    }
    return 0;
}

int all_ships_down(struct ship *ship_arr) { // returns an int representing whether all ships have been sunk
    for (int i = 0; i < 5; i++) {
        if (ship_arr[i].ship_sunk == 0) {
            return 0; 
        }
    }
    return 1;
}

struct ship *create_ships() { // mallocs/creates an array of all 5 ships 
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

int place_ship_on_grid(char **grid, int row, int col, char dir, int len) { // given a grid, starting place, direction, and length, plots a ship
    if (grid[row][col] != '-'){ //if starting coord has collision
        return 0;
    } 

    for (int i = 0; i < len; i++){ // this loop checks for collisions
        if (dir == 'H' || dir == 'h') {
            if ((col + i) > 9 || grid[row][col + i] != '-') { 
                return 0;
            }
        } else if (dir == 'V' || dir == 'v'){
            if ((row + i) > 9 || grid[row + i][col] != '-') {
                return 0;
            }
        }
    }

    for (int i = 0; i < len; i++) { // this loop places the ships on the grid
        if (dir == 'H' || dir == 'h'){
            grid[row][col + i] = 'O';
        }
        else if (dir == 'V' || dir == 'v'){
            grid[row + i][col] = 'O';
        }             
    }
    return 1;            
}

void random_ship_placement(struct ship *ship_arr, char **grid) { // places all 5 ships randomly on the grid
    char directions[] = {'H', 'V', '\0'}; 
    srand(time(NULL)); 

    for (int i = 0; i < ALL_SHIPS; i++) { // for each ship
        ship_arr[i].hits = 0;
        ship_arr[i].ship_sunk = 0; 
        int filled = 0; // checks for potential collisions 

        while (filled == 0) { //while ship has not been placed
            int row = rand() % 10; //generate a random row and col
            int col = rand() % 10;
            char direction = directions[rand() % 2]; // choose a random direction

            ship_arr[i].start.row = row; 
            ship_arr[i].start.col = col;
            ship_arr[i].direction = direction;

            if (place_ship_on_grid(grid, row, col, direction, ship_arr[i].length) == 1) { //try to place the ship
                filled = 1;
            }
        }
    }
}

int valid_guess(char *input){ // determines whether row & col input is valid 
    if (strlen(input) != 1 || input[0] < '0' || input[0] > '9') {
        return 0;
    } else {
        return 1;
    }
}

void player_ship_placement(struct ship* ship_arr, char **grid){ //takes user input to place all 5 ships on the grid
    for (int i = 0; i < ALL_SHIPS;  i++){
        int done = 0;
        char input[10];
        ship_arr[i].hits = 0;
        ship_arr[i].ship_sunk = 0; 

        sleep(1);
        printf("Filling %s (length %d): \n", ship_arr[i].name, ship_arr[i].length);

        while (done == 0) { //input validation
        ROW:
            printf("Enter a row (0-9): ");
            fgets(input, sizeof(input), stdin);
            check_new_line(input);         
            if (valid_guess(input) == 0) {
                printf("Invalid input. Please enter a single digit row between 0 and 9.\n");
                goto ROW;
            }
            ship_arr[i].start.row = input[0] - '0';

        COL:
            printf("Enter a column (0-9): ");
            fgets(input, sizeof(input), stdin);
            check_new_line(input);
            if (valid_guess(input) == 0) {
                printf("Invalid input. Please enter a single digit column between 0 and 9.\n");
                goto COL;
            }
            ship_arr[i].start.col = input[0] - '0';

        DIR:
            printf("Enter a direction ('V' for vertical, 'H' for horizontal): ");
            fgets(input, sizeof(input), stdin);
            check_new_line(input);
            if (strlen(input) != 1 || (input[0] != 'V' && input[0] != 'v' && input[0] != 'H' && input[0] != 'h')) {
                printf("Invalid input. Please enter a valid direction ('H' or 'V').\n\n");
                goto DIR;
            }
            ship_arr[i].direction = input[0];

            if (place_ship_on_grid(grid, ship_arr[i].start.row, ship_arr[i].start.col, ship_arr[i].direction, ship_arr[i].length) == 1) { //tries to place ship
                done = 1;
                printf("\n%s successfully placed.\n\n", ship_arr[i].name);
                sleep(1);
                printf("Player grid: ");
                print_grid(grid);
            } else {
                sleep(1);
                printf("\nShip collision. Try again. \n\n");
            }            
        }
    }   
    printf("All ships placed!\n\nTime to play!\n\n");
}

void hit_which_ship(int x, int y, struct ship *ships, char **grid) { //determines which ship on the grid was hit 
    for (int i = 0; i < 5; i++) { // checking each ship
        if (x == ships[i].start.row && y == ships[i].start.col) { // if it hits the starting coordinates, then automatically done
            ships[i].hits++;
            ship_down_check(&ships[i], ships, grid);
            break;
        } else if (ships[i].direction == 'V' || ships[i].direction == 'v') {
            for (int j = 1; j < ships[i].length; j++) {
                if (y == ships[i].start.col && x == (ships[i].start.row + j)) {
                    ships[i].hits++;
                    ship_down_check(&ships[i], ships, grid);
                    break;
                }
            } 
        } else if (ships[i].direction == 'H' || ships[i].direction == 'h') {
            for (int j = 1; j < ships[i].length; j++) {
                if (x == ships[i].start.row && y == (ships[i].start.col + j)) {
                    ships[i].hits++;
                    ship_down_check(&ships[i], ships, grid);
                    break;
                }
            }
        }
    }
}

char player_hit_or_miss(char** grid1, char** grid2, int row, int col, struct ship* ships){ // determines whether a player guess is a hit or miss
    if (grid1[row][col] == 'O') {
        grid1[row][col] = 'H';
        grid2[row][col] = 'H';
        sleep(1);
        printf("\nGuess at row %d, column %d was a...", row, col);
        printf(" hit! marked with an H (hit)\n");
        hit_which_ship(row, col, ships, grid2);
        printf("Enemy grid: ");
        print_grid(grid2);
        return 'H';
    } else if (grid1[row][col] == '-') {
        grid1[row][col] = 'M';
        grid2[row][col] = 'M';
        sleep(1);
        printf("\nGuess at row %d, column %d was a...", row, col);
        printf(" miss! marked with an M (miss)\n");
        printf("\nEnemy grid: ");
        print_grid(grid2);
        return 'M';
    } else {
        return 'F'; //failure 
    }
}

char fill_player_guess(char **ships_grid, struct ship *ships, char **visible_grid) { // takes player input for guess
    int row; 
    int col;
    int valid = 0;
    char input[10];

    while (valid == 0) {
    ROW2: // validate row input
        printf("Enter a row number (0-9): ");
        fgets(input, sizeof(input), stdin); 
        check_new_line(input); 

        if (strlen(input) != 1 || input[0] < '0' || input[0] > '9') {
            printf("Invalid input. Please enter a single digit row between 0 and 9.\n");
            goto ROW2;
        }
        sscanf(input, "%d", &row); 

    COL2: //validate col input
        printf("Enter a column number (0-9): ");
        fgets(input, sizeof(input), stdin); 
        check_new_line(input); 

        if (strlen(input) != 1 || input[0] < '0' || input[0] > '9') {
            printf("Invalid input. Please enter a single digit column between 0 and 9.\n");
            goto COL2;
        }
        sscanf(input, "%d", &col); 

        if (ships_grid[row][col] != '-' && ships_grid[row][col] != 'O') {
            printf("Coordinates already guessed. Enter a new set of coordinates.\n");
            goto ROW2; //start over
        } else {
            valid = 1;
        }
    }
    return player_hit_or_miss(ships_grid, visible_grid, row, col, ships);
}

struct guess computer_hit_or_miss(char **grid, int row, int col, struct ship* ships){ // determines whether a computer guess is a hit or miss
    struct guess computer_guess;
    printf("AI guess: row %d, col %d", row, col);
    
    if (grid[row][col] == '-'){
        printf(" (miss) \n");
        grid[row][col] = 'M';
        computer_guess.row = -1;
        computer_guess.col = -1;
    } else { //already validated that guess is hit
        printf(" (hit) \n");
        grid[row][col] = 'H';
        hit_which_ship(row, col, ships, grid);
        computer_guess.row = row;
        computer_guess.col = col;
    }

    return computer_guess;
}

struct guess computer_guess(char **player_grid, struct ship *player_ships){ // computer makes a random guess; if it hits, return coords, if it misses, returns -1,-1
    int done = 0; 
    int row;
    int col;

    while (done == 0){ 
        row = rand() % 10; //generate a random row and col
        col = rand() % 10;
        if (player_grid[row][col] != 'O' && player_grid[row][col] != '-'){
            continue;
        }
        done = 1;
    }

    return computer_hit_or_miss(player_grid, row, col, player_ships);
}

struct guess smart_computer_guess(struct ship* player_ships, char **player_grid, struct guess last_guess){ // computer guess - adjacent to a previous hit
    int possible_rows[] = {(last_guess.row)-1, last_guess.row, (last_guess.row)+1 };
    int possible_cols[] = {(last_guess.col)-1, last_guess.col, (last_guess.col)+1 };

    int row;
    int col;
    int done = 0;

    while (done == 0){ //validates guess, makes sure it doesn't go off grid
        row = possible_rows[rand() % 3]; // guess x is in range of x-1 < x < x+1
        col = possible_cols[rand() % 3];// guess y is in range of y-1 < y < y+1  
        if ((row < 0 || row > 9) || (col < 0 || col > 9)) {
            continue;
        } else if (player_grid[row][col] != '-' && player_grid[row][col] != 'O') {
            continue;
        } else {
            done = 1;
        }
    } 
    return computer_hit_or_miss(player_grid, row, col, player_ships);
}

char **grid_maker() { //mallocs and fills a 10x10 grid 
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

void practice_game(){ //upper level function for practice game
    int misses = 0; // tracks the number of misses  
    int guess_limit; //practice mode guess limit - user gets to choose how many tries they'd like to get 
    char hit_or_miss;
    char input[10];
    sleep(1);

GUESS:
    printf("Enter a guess limit: "); //validates guess user input
    fgets(input, sizeof(input), stdin);
    check_new_line(input);
    
    if (sscanf(input, "%d", &guess_limit) != 1 || guess_limit <= 0) {
        printf("Invalid input. Please enter a positive number.\n");
        goto GUESS;
    }

    char **computer_grid = grid_maker(); // two grids - one tracks the data and another displays hits to the users 
    char **player_visible_grid = grid_maker();
    struct ship *all_ships = create_ships();

    random_ship_placement(all_ships, computer_grid); 

    sleep(1);
    printf("\nTime to play!\n\n");
    while (all_ships_down(all_ships) == 0 && guess_limit != misses) { //conditions for ending game 
        sleep(1);
        hit_or_miss = fill_player_guess(computer_grid, all_ships, player_visible_grid);
        if (hit_or_miss == 'M'){
            misses++;
            printf("%d guess(es) remain.\n\n", guess_limit-misses);
        }
    }

    if (guess_limit == misses){
        printf("You lose! Game over.\n\nEnemy ships:");
        print_grid(computer_grid);
    } else {
        printf("You have successfully sunk all enemy ships! You win.");
    }

    free_grid(player_visible_grid); //frees all malloced space 
    free_grid(computer_grid);
    free_ships(all_ships);
}

void AI_game(){ //upper level function for ai game
    struct guess computer;
    computer.row = -1;
    computer.col = -1;

    char **player_grid = grid_maker(); //player's personal grid
    char **AI_grid = grid_maker(); // computer's grid
    char **AI_visible_grid = grid_maker(); // version of computer grid that player is allowed to see
    struct ship *player_ships = create_ships();
    struct ship *AI_ships = create_ships();


    player_ship_placement(player_ships, player_grid);
    random_ship_placement(AI_ships, AI_grid); //randomly fill the computer's grid
 
   
    while (all_ships_down(player_ships) == 0 && all_ships_down(AI_ships) == 0){
        fill_player_guess(AI_grid, AI_ships, AI_visible_grid);
        sleep(3);

        if (computer.row == -1 && computer.col == -1){
            computer = computer_guess(player_grid, player_ships);
        }
        else {
            computer = smart_computer_guess(player_ships, player_grid, computer);
        }  
        printf("\nPlayer grid: ");
        print_grid(player_grid);
    }

    if (all_ships_down(player_ships) == 1){
        printf("\nAll of your ships have been sunk! You lose.\n");
    } else {
        printf("\n You have successfully sunk all enemy ships! You win!\n");
    }

    free_grid(player_grid);
    free_grid(AI_grid);
    free_grid(AI_visible_grid);

    free_ships(player_ships);
    free_ships(AI_ships);
}

void play_game() { // play game --> chooses between game modes 
    int mode = game_instructions();
    printf("Press enter/return to continue\n");
    clear_input_buffer(); // Clear any remaining input in the buffer

    if (mode == PRACTICE_MODE) {
       practice_game();
    } else if (mode == AI_MODE) {
        AI_game();
    }
}

int main() {
    play_game();
}
