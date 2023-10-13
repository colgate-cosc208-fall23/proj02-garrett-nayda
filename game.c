/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>

struct ship{ 
    int startingX; // must be 0-9
    int startingY; //must be 0-9
    char direction; // u, d, l, or r
    int length; // length of the ship, note that we already have the first coord
};

struct ship* createShips(){
    struct ship* allShips = malloc(5*sizeof(struct ship*));
    
    struct ship *patrolBoat = malloc(sizeof(struct ship));
    allShips[0] = *patrolBoat;
    struct ship *submarine =  malloc(sizeof(struct ship));
    allShips[1] = *submarine;
    struct ship *destroyer =  malloc(sizeof(struct ship));
    allShips[2] = *destroyer;
    struct ship *battleship =  malloc(sizeof(struct ship));
    allShips[3] = *battleship;
    struct ship *carrier =  malloc(sizeof(struct ship));
    allShips[4] = *carrier;
    return allShips;
}

void fillPlayerGuess(char** grid){
    char x[1];
    char y[1];
    printf("Please enter an X coordinate guess between 0 and 9\n");
    fgets(x,1,stdin);
    printf("Please enter a Y coordinate guess between 0 and 9\n");
    fgets(y,1,stdin);

    int valX = atoi(x);
    int valY = atoi(y);

    if ((valX<0 || valX>9) || (valY<0 || valY>9)){
        printf("Please enter an x or y coordinate between 0 and 9\n");
        //need to get inputs again. While loop until both are 0-9?
    }
    if (grid[valX][valY] == 'M' || grid[valX][valY] == 'H'){
        printf("You have already guessed this coordinate, try another!\n"); 
    }
    else if (grid[valX][valY] == 'O' ){
        grid[valX][valY] = 'H';
        printf("Guess at (%d,%d) was a hit! Marked with an H (Hit)!\n",valX,valY);
    }
    else if (grid[valX][valY] == '_'){
        grid[valX][valY] = 'M';
        printf("Guess at (%d,%d) was not a hit. Marked with an M (Miss).\n",valX,valY);
    }
}


void fillShipCoords(struct ship* shipArr, char** grid){
    for (int i  = 0; i < 5; i++){
        int startFilled = 0; // checks if the starting coord is filled yet 

        while (startFilled == 0){
            int x = 0 + rand() % (9 - 0 + 1); // maybe this works? chatGPT
            int y =  0 + rand() % (9 - 0 + 1); 

            if (grid[x][y] == ' '){
                grid[x][y] = 'O';
                startFilled = 1;
            }          
        }
    }
}


char** playerGrid(){
    char **grid = malloc(sizeof(char *) *11);
    grid[10] = NULL;
    for (int i = 0; i<10; i++){
        grid[i] = malloc(sizeof(char) * 11);
        for (int j = 0; j < 10; j++){
            grid[i][j] = '_';
        }
        grid[i][10] = '\0';
    }
    return grid;
} 


void printGrid(char **grid){
    for (int i = 0; i < 10; i++){
        printf("%s\n", grid[i]);
    }
}

char** shipStatus(){
    char** ships = malloc(sizeof(char *) *6);
    ships[5] = NULL;
    int shipSizes[] = {2, 3, 3, 4, 5}; //size of ships

    for (int i = 0; i < 5; i++){
        ships[i] = malloc(sizeof(char) * shipSizes[i]+1);
        ships[i][shipSizes[i]] = '\0';
    }
    return ships;
}

void placeShips(char** grid){

}

int main() {
    printf("Working to main\n");  
    char** grid = playerGrid();
    printGrid(grid);

}