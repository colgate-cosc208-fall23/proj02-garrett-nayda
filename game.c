/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
char** playerGrid(){
    char **grid = malloc(sizeof(char *) *11);
    grid[10] = NULL;
    for (int i = 0; i<10; i++){
        grid[i] = malloc(sizeof(char) * 11);
        for (int j = 0; j < 10; j++){
            grid[i][j] = ' ';
        }
        grid[i][10] = '\0';
    }
    return grid;
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
    
}