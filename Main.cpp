#include <iostream>
#include <string>

#include "API.h"

// direction
char dir_chars[4] = {'n', 'e', 's', 'w'};

// allows you to use bitwise OR when updating which walls are present in a cell
// also allows you to bitwise AND
int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};

enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

enum DirectionBitmask {
    NORTH_MASK = 0b1000,
    EAST_MASK  = 0b0100,
    SOUTH_MASK = 0b0010,
    WEST_MASK  = 0b0001
};

struct Coord {
    int x;
    int y;
};

struct Cell {
    Coord pos;
    Direction dir;
};

struct CellList {
    int size;
    Cell* cells;
};

struct Queue {
    // Cell object array with max size
    // Two int objects for head and tail
    Cell* objects;
    int head;
    int tail;
    int max_size;
};

struct Maze {
    Coord mouse_pos;
    Direction mouse_dir;

    // 2, 2D arrays for distances of each cell, and values for cell walls

    Coord* goalPos;
};

// Queue functions
void initQueue(Queue *q) { //initalize empty queue
    while (q != NULL)
        q++;
        printf("%i %i %i ", (*q).head, (*q).tail, (*q).max_size);
}

bool isQEmpty(Queue q) {
    return q.head == q.tail;
}



// Maze functions
void scanWalls(Maze* maze) { // fill in code for changing value of the cell walls
    if (API::wallFront()) {
        
    }
    if (API::wallRight()) {
        
    }
    if (API::wallLeft()) {
        
    }
}

// void updateSimulator(Maze maze) { // redraws the maze in simulator after each loop in main
//     for(int x = 0; x < MAZE_SIZE; x++) 
//     {
//         for(int y = 0; y < MAZE_SIZE; y++) 
//         {
//             if (maze.cellWalls[y][x] & NORTH_MASK)
//                 // API set walls for some direction
//             if (maze.cellWalls[y][x] & EAST_MASK)
//                 // API set walls for some direction
//             if (maze.cellWalls[y][x] & SOUTH_MASK)
//                 // API set walls for some direction
//             if (maze.cellWalls[y][x] & WEST_MASK)
//                 // API set walls for some direction
//         }
//     }
// }

void updateMousePos(Coord* pos, Direction dir) {
    //depending on the mouse direction, increment position by one
    if (dir == NORTH)
        // increment in some direction
    if (dir == SOUTH)
        // increment in some direction
    if (dir == WEST)
        // increment in some direction
    if (dir == EAST)
        // increment in some direction
    return;
}

// CellList* getNeighborCells(Maze* maze, Coord c) { //to be called in a while loop within Floodfill when setting each cell
//     return ;
// };

void floodFill(Maze* maze, bool to_start) { // function to be called everytime you move into a new cell
    return;
}

int main(){
    
}