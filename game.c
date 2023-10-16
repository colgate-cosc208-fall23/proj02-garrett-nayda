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
int gameOver(){
    return 0;
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
    struct coord shipCoords[];
};



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
    printf("direction is %c, length is %d \n", dir, len);
    int lengthAfterStart = len-1;
    if (grid[x][y] == '-'){    
        if (dir == 'l'){
            for (int i = 1; i <= lengthAfterStart; i++){
                if ((y-i) < 0 || grid[x][y-i] != '-'){
                    printf("NOT CLEAR IN LEFT DIRECTION");
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
                    printf("NOT CLEAR IN RIGHT DIRECTION");
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
                    printf("NOT CLEAR IN UP DIRECTION");
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
                    printf("NOT CLEAR IN DOWN DIRECTION");
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
    srand(time(NULL));

    for (int i  = 0; i < 5; i++){ //for each ship
        shipArr[i].hits = 0;
        shipArr[i].shipDown = 1;
        
        int filled = 0; // checks if ship has been put on grid yet
        for (int j = 0; j < shipArr[i].length; j++) {
            while (filled == 0){
                int x = rand()%10; 
                int y =  rand() %10; 
                char direction = directions[rand()%4]; // random 0-3


                shipArr[i].shipCoords[j].x = x; //comment these out if not working 
                shipArr[i].shipCoords[j].y = y; //Fills the coords array struct in each ship

                printf("%d x, %d y \n", x, y);
                
                if (checkIfGridClear(grid, x, y, direction, shipArr[i].length) == 1){
                    filled = 1;
                    printGrid(grid);
                    printf("SHIP %d done\n", i+1);
                }
                else{
                    printf("trying other coords\n");
                }
            }
        }          
    }

    printf("all ships filled");
}



void fillPlayerGuess(char** grid){
    char x[3];
    char y[3];
    int valX;
    int valY;
    struct coord *aCoord = malloc(sizeof(struct coord));

    printf("X COORD GUESS: ");
    fgets(x, 3, stdin);
    valX = atoi(x);

    printf("Y COORD GUESS: ");
    fgets(y, 3, stdin);
    valY = atoi(y);
    //coords[0] = valX;
    //coords[1] = valY;

    if ((valX<0 || valX>9) || (valY<0 || valY>9)){
        printf("INVALID INPUT: GRID REMAINS UNCHANGED\n");
        //need to get inputs again. While loop until both are 0-9?
    }
    else if(grid[valX][valY] == 'M' || grid[valX][valY] == 'H'){
        printf("COORDINATE ALREADY GUESSED\n"); 
    }
    else if (grid[valX][valY] == 'O' ){
        grid[valX][valY] = 'H';
        printf("Guess at (%d,%d) was a hit! Marked with an H (Hit)!\n",valX,valY);
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

    while (gameOver() == 0){
        fillPlayerGuess(computerGrid);
        printGrid(computerGrid);
    }
  

}

int main() { 
    playGame();
    
}