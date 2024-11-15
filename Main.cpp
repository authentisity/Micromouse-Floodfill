#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "API.h"

// direction
char dir_chars[4] = {'n', 'e', 's', 'w'};

const int MAZE_SIZE = 8;

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

const Coord GOAL_SQUARES[4] = {{3, 3}, {3, 4}, {4, 3}, {4, 4}};

bool operator == (const Coord &coord1, const Coord &coord2)
{
   return coord1.x == coord2.x && coord1.y == coord2.y;
}

struct Cell {
    Coord pos;
    uint8_t cellWalls;
    uint8_t dist;
    uint8_t cellType; // x0000: unchecked, x0001: checked, x0010: goal cell
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
    Cell cells[8][8];
};

Maze* initMaze(void){
    Maze* maze = (Maze*)malloc(sizeof(Maze));
    for (int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j < MAZE_SIZE; j++){
            maze->cells[i][j].cellWalls = 0;
            maze->cells[i][j].dist = 255;
            maze->cells[i][j].pos = {i, j};
            maze->cells[i][j].cellType = (std::find(std::begin(GOAL_SQUARES), std::end(GOAL_SQUARES), (Coord)maze->cells[i][j].pos) != std::end(GOAL_SQUARES)) ? 2 : 0;

            printf("[%i, %i, %i, %i, %i]", maze->cells[i][j].cellWalls, maze->cells[i][j].dist, maze->cells[i][j].pos.x, maze->cells[i][j].pos.y, maze->cells[i][j].cellType);
        }
    }
    return maze;
}

// Queue functions
void initQueue(Queue *q) { //initalize empty queue
    q->head = 0;
    q->tail = 0;
    q->max_size = 64;
    
    q->objects = (Cell *)malloc(sizeof(Cell) * q->max_size);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            q->objects[i + 8 * j].pos = {i, j};
        }
    }

    // for(int i = 0; i < q->max_size; i++){
    //     std::cout << "Cell " << i << " initialized at (" << q->objects[i].pos.x << ", " << q->objects[i].pos.y << ") with direction " << q->objects[i].dir << std::endl;
    // }
    // printf("\n%i %i %i", q->head, q->tail, q->max_size);
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

void updateSimulator(Maze maze) { // redraws the maze in simulator after each loop in main
    for(int x = 0; x < MAZE_SIZE; x++) 
    {
        for(int y = 0; y < MAZE_SIZE; y++) 
        {
            if (maze.cells[y][x].cellWalls & NORTH_MASK)
                // API set walls for some direction
            if (maze.cells[y][x].cellWalls & EAST_MASK)
                // API set walls for some direction
            if (maze.cells[y][x].cellWalls & SOUTH_MASK)
                // API set walls for some direction
            if (maze.cells[y][x].cellWalls & WEST_MASK){}
                // API set walls for some direction
        }
    }
}

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

void floodFill(Maze* maze, int sr = NULL, int sc = NULL) { // function to be called everytime you move into a new cell
    sr = maze->mouse_pos.x;
    sc = maze->mouse_pos.y;
    
    return;
}

int main(){
    Queue temp = {NULL, 0, 0, 0};
    initQueue(&temp);

    Maze* maze = initMaze();
    Coord coord = {0, 0};

    // while(!isQEmpty(temp)){
    //     floodFill(&maze, 1);
    //     updateMousePos(&coord, NORTH);
    // }
}