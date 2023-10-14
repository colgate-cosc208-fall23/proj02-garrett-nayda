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
    int hits; 
    int shipDown; 

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
    char x[3];
    char y[3];
    int valX;
    int valY;

    printf("Please enter an X coordinate guess between 0 and 9\n");
    fgets(x, 3, stdin);
    valX = atoi(x);

    printf("\nPlease enter a Y coordinate guess between 0 and 9\n");
    fgets(y, 3, stdin);
    valY = atoi(y);

    


    

    if ((valX<0 || valX>9) || (valY<0 || valY>9)){
        printf("Please enter an x or y coordinate between 0 and 9\n");
        //need to get inputs again. While loop until both are 0-9?
    }
    else if(grid[valX][valY] == 'M' || grid[valX][valY] == 'H'){
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
        shipArr[i].hits = 0;
        shipArr[i].shipDown = 1;
        int filled = 0; // checks if the starting coord is filled yet 

        while (filled== 0){
            int x = 0 + rand() % (9 - 0 + 1); // chatGPT for this equation 
            int y =  0 + rand() % (9 - 0 + 1); 
            // create a direction
            // check if grid is clear 
            
            }          
        }


    }


int checkIfGridClear(char** grid, int x, int y, char dir, int len){
    int lengthAfterStart = len-1;
    if (grid[x][y] == '_'){    
        if (dir == 'l'){
            for (int i = 1; i <= lengthAfterStart; i++){
                if (grid[x][y-i] != '_'){
                    printf("NOT CLEAR IN LEFT DIRECTION");
                    return 0; 
                }
            }
        }
        else if (dir == 'r'){
               for (int i = 1; i <= lengthAfterStart; i++){
                if (grid[x][y+i] != '_'){
                    printf("NOT CLEAR IN RIGHT DIRECTION");
                    return 0; 
                }

            }
        }
        else if(dir == 'u'){
               for (int i = 1; i <= lengthAfterStart; i++){
                if (grid[x-i][y] != '_'){
                    printf("NOT CLEAR IN UP DIRECTION");
                    return 0; 
                }
            }
        }
        else if (dir == 'd'){
               for (int i = 1; i <= lengthAfterStart; i++){
                if (grid[x+i][y] != '_'){
                    printf("NOT CLEAR IN DOWN DIRECTION");
                    return 0; 
                }
            }
        }
    
        return 1; 
    }
    return 0;
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
    fillPlayerGuess(grid);
    printGrid(grid);

}