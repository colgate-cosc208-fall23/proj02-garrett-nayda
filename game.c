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
    printf("Welcome to Battleship...\n");
    printf("\n");
    printf("Player Instructions: \n");
    printf("Your goal is to sink all 5 ships!\n");
    printf("Each turn, you will be given the chance to guess an x and y coordinate...\n");
    printf("\n");
    printf("If your guess hits a ship, it will be marked with an 'H' for Hit!\n");
    printf("Likewise, a missed guess will be marked with an 'M for Miss on the grid.\n");


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

int shipsLeft(struct ship* shipArr){
     int downCt = 0;
    for (int i = 0; i < 5; i++){
        if (shipArr[i].shipDown == 1){
            downCt++;
        }  
    }
    return 5-downCt;  
}

int shipDown(struct ship* ship, struct ship* shipArr){
    if (ship->length == ship->hits){
        ship->shipDown = 1;
        printf("Ship Sunk! %d ships remaining.\n", shipsLeft(shipArr));
        return 1;
    }
    return 0;
}


int gameOver(struct ship* shipArr){
    int downCt = 0;
    for (int i = 0; i < 5; i++){
        if (shipArr[i].shipDown == 1){
            downCt++;
        }  
    }
    if (downCt == 5){
        printf("You win! You have successfully sunk all 5 ships!");
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
            shipDown(&ships[i],ships);
            break;
        }
        else if (ships[i].direction == 'u') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valY == ships[i].coords.y && valX == (ships[i].coords.x - j)) {
                    ships[i].hits++;
                    shipDown(&ships[i], ships);
                    break;
                }
            }
        }
        else if (ships[i].direction == 'd') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valY == ships[i].coords.y && valX == (ships[i].coords.x + j)) {
                    ships[i].hits++;
                    shipDown(&ships[i],ships);
                    break;
                }
            }
        }
        else if (ships[i].direction == 'l') {
            for (int j = 1; j < ships[i].length; j++) {
                if (valX == ships[i].coords.x && valY == (ships[i].coords.y - j)) {
                    ships[i].hits++;
                    shipDown(&ships[i], ships);
                    break;
                }
            }
        }
        else if (ships[i].direction == 'r') {
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


void fillPlayerGuess(char** grid, struct ship* ships, char** playerGrid, int* missCt){
    char x[3];
    char y[3];
    int valX;
    int valY;

    printf("x: ");
    fgets(x, 3, stdin);
    valX = atoi(x);

    printf("y: ");
    fgets(y, 3, stdin);
    valY = atoi(y);

    if ((valX<0 || valX>9) || (valY<0 || valY>9)){
        printf("Invalid input! Try again.\n");
        //need to get inputs again. While loop until both are 0-9?
    }
    else if(grid[valX][valY] == 'M' || grid[valX][valY] == 'H'){
        printf("Coordinate already guessed! Try again.\n"); 
    }
    else if (grid[valX][valY] == 'O' ){
        grid[valX][valY] = 'H';
        playerGrid[valX][valY] = 'H';
        hitWhichShip(valX, valY, ships);
        printf("Hit at (%d,%d)!  Marked with an H (Hit)!\n",valX,valY);
    }
    else if (grid[valX][valY] == '-'){
        grid[valX][valY] = 'M';
        playerGrid[valX][valY] = 'M';
        *missCt++;
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
    int* missCt = malloc(sizeof(int*));
    *missCt = 0;

    char** computerGrid = gridMaker();
    char** playerVisibleGrid = gridMaker(); 
    struct ship* allShips = createShips();
    fillShipCoords(allShips, computerGrid);

    while (gameOver(allShips) == 0){
        fillPlayerGuess(computerGrid, allShips, playerVisibleGrid, missCt);
        printf("MISS CT: %d\n", *missCt);
        printGrid(playerVisibleGrid);
    }
  

}

int main() { 
    playGame();
    
}