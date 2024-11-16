#include "API.h" 

#define MAZE_SIZE 8
#define MAX_QUEUE_SIZE 100
#define NULL_CELL {{-1, -1}, -1, -1}

const int GOAL_COORDS[4][2] = {{3, 3}, {3, 4}, {4, 3}, {4, 4}};
// direction
const char dir_chars[4] = {'n', 'e', 's', 'w'};

// allows you to use bitwise OR when updating which walls are present in a cell
// also allows you to bitwise AND
const int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};

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

bool operator == (Coord c1, Coord c2){
    return c1.x == c2.x && c1.y == c2.y;
}

struct Cell {
    Coord pos;
    int distance;
    int cellWalls;
};

bool operator == (Cell c1, Cell c2){
    return c1.pos == c2.pos && c1.distance == c2.distance && c1.cellWalls == c2.cellWalls;
} 

struct CellList {
    int size;
    Cell* cells;
};

struct Queue {
    // Cell object array with max size
    // Two int objects for head and tail
    Cell objects[MAX_QUEUE_SIZE];
    int head;
    int tail;
};

struct Maze {
    Coord mouse_pos;
    Direction mouse_dir;

    // 2, 2D arrays for distances of each cell, and values for cell walls
    Cell cells[MAZE_SIZE][MAZE_SIZE];
    Coord* goalPos;
};

// Queue functions
void initQueue(Queue *q) { //initalize empty queue
    q->head = q->tail = 0;
}

bool isQEmpty(Queue* q) {
    return q->head == q->tail;
}

void enqueue(Queue* q, Cell item)
{
    if (q->head == MAX_QUEUE_SIZE) q->head = 0;
    q->objects[q->head] = item;
    q->head++;
}

Cell dequeue(Queue* q)
{
    if (isQEmpty(q)) {
        return NULL_CELL;
    }
    else if (q->tail == MAX_QUEUE_SIZE) q->tail = 0;
    return q->objects[q->tail++];
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
            if (maze.cells[y][x].cellWalls & WEST_MASK)
                // API set walls for some direction
                break;
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

CellList* getNeighborCells(Maze* maze, Coord c) { //to be called in a while loop within Floodfill when setting each cell
    
};

void floodFill(Maze* maze, bool to_start) { // function to be called everytime you move into a new cell
    
}

int main(){
    struct Queue* q = (Queue*)malloc(sizeof(Queue*));
    initQueue(q);
    enqueue(q, {{50, 1}, -1, -1});
    printf("%i", dequeue(q).cellWalls);
}