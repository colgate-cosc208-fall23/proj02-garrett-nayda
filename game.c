/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 


void printGrid(char **grid){
    for (int i = 0; i < 10; i++){
        printf("%s\n", grid[i]);
    }
}

void gameInstructions(){
    printf("WELCOME TO BATTLESHIP. (GAME INSTRUCTIONS).\n");
    printf("COORDINATE GUESSES MUST BE BETWEEN 0-9.\n");
}


struct coord{
    int x;
    int y;
};





struct ship{ 
    int startingX; // must be 0-9
    int startingY; //must be 0-9
    char direction; // u, d, l, or r
    int length; // length of the ship, note that we already have the first coord
    int hits; 
    int shipDown; 
    struct coord coords;
};

int shipDown(struct ship ship){
    printf("%d hits, %d length\n", ship.hits, ship.length);
    if (ship.length == ship.hits){
        printf("SHIP DOWN.");
        ship.shipDown = 1;
        return 1;
    }
    return 0;
}


int gameOver(struct ship* shipArr){
    int downCt = 0;
    for (int i = 0; i < 5; i++){
        if (shipArr[i].shipDown == 1){
            downCt++;
            printf("Down count: %d\n",downCt);
        }
    }
    if (downCt == 5){
        printf("GAME OVER! YOU WIN");
        return 1;
    }

    return 0;
}


struct ship* createShips(){
    struct ship* allShips = malloc(5*sizeof(struct ship*));
    
    struct ship *patrolBoat = malloc(sizeof(struct ship));
    patrolBoat->length = 2;
    allShips[0] = *patrolBoat;

    struct ship *submarine =  malloc(sizeof(struct ship));
    submarine->length = 3;
    allShips[1] = *submarine;

    struct ship *destroyer =  malloc(sizeof(struct ship));
    destroyer->length = 3;
    allShips[2] = *destroyer;

    struct ship *battleship =  malloc(sizeof(struct ship));
    battleship->length = 4;
    allShips[3] = *battleship;

    struct ship *carrier =  malloc(sizeof(struct ship));
    carrier->length = 5;
    allShips[4] = *carrier;

    return allShips;
}

int checkIfGridClear(char** grid, int x, int y, char dir, int len){
    int lengthAfterStart = len-1;
    if (grid[x][y] == '-'){    
        if (dir == 'l'){
            for (int i = 1; i <= lengthAfterStart; i++){
                if ((y-i) < 0 || grid[x][y-i] != '-'){
                    return 0; 
                }
            }
            for (int i = 0; i < len; i++){
                grid[x][y-i] = 'O';
            }             
        }
        else if (dir == 'r'){
               for (int i = 1; i <= lengthAfterStart; i++){
                if ((y+i)>9 || grid[x][y+i] != '-'){
                    return 0; 
                }
            }
            for (int i = 0; i < len; i++){
                grid[x][y+i] = 'O';
            }
                 
        }
        else if(dir == 'u'){
               for (int i = 1; i <= lengthAfterStart; i++){
                if ((x-i) < 0 || grid[x-i][y] != '-'){
                    return 0; 
                }
            }
            for (int i = 0; i < len; i++){
                grid[x-i][y] = 'O';
            }
                 
        }
        else if (dir == 'd'){
               for (int i = 1; i <= lengthAfterStart; i++){
                if ((x+i) > 9 || grid[x+i][y] != '-'){
                    return 0; 
                }
            }
            for (int i = 0; i < len; i++){
                grid[x+i][y] = 'O';
            }
                 
        }
        return 1; 
    }
    return 0;
}

void fillShipCoords(struct ship* shipArr, char** grid){
    char directions[] = {'u', 'd', 'l', 'r', '\0'}; //creates an array for the possible directions 
    srand(time(NULL)); //used CHATGPT for this

    for (int i  = 0; i < 5; i++){ //for each ship
        shipArr[i].hits = 0;
        shipArr[i].shipDown = 0;    
        int filled = 0; // checks if ship has been put on grid yet

        while (filled == 0){
            int x = rand()%10; 
            int y =  rand() %10; 
            char direction = directions[rand()%4]; // random 0-3

            shipArr[i].coords.x = x; //comment these out if not working 
            shipArr[i].coords.y = y; //Fills the coords array struct in each ship
            shipArr[i].direction = direction;
          
            if (checkIfGridClear(grid, x, y, direction, shipArr[i].length) == 1){
                filled = 1;
            }
        }      
    }
}


void hitWhichShip(int valX, int valY, struct ship* ships) {
    for (int i = 0; i < 5; i++) { // checking each ship
        if (valX == ships[i].coords.x && valY == ships[i].coords.y) { // if it hits the starting coordinates, then automatically done
            ships[i].hits++;
            shipDown(ships[i]);
            break;
        }
        else if (ships[i].direction == 'u') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valY == ships[i].coords.y && valX == (ships[i].coords.x - j)) {
                    ships[i].hits++;
                    shipDown(ships[i]);
                    break;
                }
            }
        }
        else if (ships[i].direction == 'd') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valY == ships[i].coords.y && valX == (ships[i].coords.x + j)) {
                    ships[i].hits++;
                    shipDown(ships[i]);
                    break;
                }
            }
        }
        else if (ships[i].direction == 'l') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valX == ships[i].coords.x && valY == (ships[i].coords.y - j)) {
                    ships[i].hits++;
                    shipDown(ships[i]);
                    break;
                }
            }
        }
        else if (ships[i].direction == 'r') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valX == ships[i].coords.x && valY == (ships[i].coords.y + j)) {
                    ships[i].hits++;
                    shipDown(ships[i]);
                    break;
                }
            }
        }
    }
}


void fillPlayerGuess(char** grid, struct ship* ships){
    char x[3];
    char y[3];
    int valX;
    int valY;

    printf("X COORD GUESS: ");
    fgets(x, 3, stdin);
    valX = atoi(x);

    printf("Y COORD GUESS: ");
    fgets(y, 3, stdin);
    valY = atoi(y);

    if ((valX<0 || valX>9) || (valY<0 || valY>9)){
        printf("INVALID INPUT: TRY AGAIN\n");
        //need to get inputs again. While loop until both are 0-9?
    }
    else if(grid[valX][valY] == 'M' || grid[valX][valY] == 'H'){
        printf("COORDINATE ALREADY GUESSED: TRY AGAIN\n"); 
    }
    else if (grid[valX][valY] == 'O' ){
        grid[valX][valY] = 'H';
        hitWhichShip(valX, valY, ships);
        printf("HIT AT (%d,%d)!  Marked with an H (Hit)!\n",valX,valY);
    }
    else if (grid[valX][valY] == '-'){
        grid[valX][valY] = 'M';
        printf("Guess at (%d,%d) was not a hit. Marked with an M (Miss).\n",valX,valY);
    }
}





char** gridMaker(){
    char **grid = malloc(sizeof(char *) *11);
    grid[10] = NULL;
    for (int i = 0; i<10; i++){
        grid[i] = malloc(sizeof(char) * 11);
        for (int j = 0; j < 10; j++){
            grid[i][j] = '-';
        }
        grid[i][10] = '\0';
    }
    return grid;
} 



void playGame(){
    gameInstructions();
    char** computerGrid = gridMaker();
    struct ship* allShips = createShips();
    fillShipCoords(allShips, computerGrid);

    while (gameOver(allShips) == 0){
        fillPlayerGuess(computerGrid, allShips);
        printGrid(computerGrid);
    }
  

}

int main() { 
    playGame();
    
}