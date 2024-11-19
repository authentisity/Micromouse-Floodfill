#include "API.h" 

#include "string.h"

#define MAZE_SIZE 16
#define MAX_QUEUE_SIZE 100
#define GOAL_SIZE 4
#define NULL_CELL {{-1, -1}, -1, -1}

const int GOAL_COORDS[GOAL_SIZE][2] = {{3, 3}, {3, 4}, {4, 3}, {4, 4}};
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

static const enum Direction direction_index[] = { NORTH, EAST, SOUTH, WEST };

enum DirectionBitmask {
    NORTH_MASK = 0b1000,
    EAST_MASK  = 0b0100,
    SOUTH_MASK = 0b0010,
    WEST_MASK  = 0b0001
};

static const enum DirectionBitmask direction_mask[] = { NORTH_MASK, EAST_MASK, SOUTH_MASK, WEST_MASK };

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

static const struct Cell EmptyCell { {0, 0}, 0, 0 };

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

void initMaze(Maze *maze) {
    maze->mouse_dir = NORTH;
    maze->mouse_pos = {0, 0};
    for(int x = 0; x < MAZE_SIZE; x++) {
        for(int y = 0; y < MAZE_SIZE; y++) {
            maze->cells[x][y] = {{x, y}, 0, 0};
            
            // printf("%i, %i, %i, %i\n", maze->cells[x][y].pos.x, maze->cells[x][y].pos.x, maze->cells[x][y].distance, maze->cells[x][y].cellWalls);
        }
    }
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
        maze->cells[maze->mouse_pos.x][maze->mouse_pos.y].cellWalls |= direction_mask[maze->mouse_dir];
    }
    if (API::wallRight()) {
        int direction_index = maze->mouse_dir + 1 > 3 ? 0 : maze->mouse_dir + 1;
        maze->cells[maze->mouse_pos.x][maze->mouse_pos.y].cellWalls |= direction_mask[direction_index];
    }
    if (API::wallLeft()) {
        int direction_index = maze->mouse_dir - 1 < 0 ? 3 : maze->mouse_dir - 1;
        maze->cells[maze->mouse_pos.x][maze->mouse_pos.y].cellWalls |= direction_mask[direction_index];
    }
}

void updateSimulator(Maze maze) { // redraws the maze in simulator after each loop in main
    for(int x = 0; x < MAZE_SIZE; x++) 
    {
        for(int y = 0; y < MAZE_SIZE; y++) 
        {
            API::setText(x, y, std::string(1, maze.cells[x][y].cellWalls + 33));
            if (maze.cells[x][y].cellWalls & NORTH_MASK)
                API::setWall(x, y, 'n');
            if (maze.cells[x][y].cellWalls & EAST_MASK)
                API::setWall(x, y, 'e');
            if (maze.cells[x][y].cellWalls & SOUTH_MASK)
                API::setWall(x, y, 's');
            if (maze.cells[x][y].cellWalls & WEST_MASK)
                API::setWall(x, y, 'w');
        }
    }
}
void changeDir(Maze* maze, Direction dir) {
    maze->mouse_dir = dir;
}

void updateMousePos(Maze* maze, Coord* pos, Direction dir) {
//     API::setText(
//     maze->mouse_pos.x,
//     maze->mouse_pos.y,
//     std::string(1, (char)(dir + '0'))
//     // std::string{static_cast<char>(maze->mouse_pos.x + '0'), ',', static_cast<char>(maze->mouse_pos.y + '0')}
// );
    //depending on the mouse direction, increment position by one
    if (maze->mouse_dir == NORTH && maze->mouse_pos.y < MAZE_SIZE - 1)
        maze->mouse_pos.y++;
    if (maze->mouse_dir == SOUTH && maze->mouse_pos.y > 0)
        maze->mouse_pos.y--;
    if (maze->mouse_dir == WEST && maze->mouse_pos.x > 0)
        maze->mouse_pos.x--;
    if (maze->mouse_dir == EAST && maze->mouse_pos.x < MAZE_SIZE - 1)
        maze->mouse_pos.x++;

        
}

CellList* getNeighborCells(Maze* maze, Coord c) { //to be called in a while loop within Floodfill when setting each cell
    CellList* cellList = (CellList*)malloc(sizeof(CellList));
    cellList->size = 0;
    cellList->cells = (Cell*)malloc(sizeof(Cell) * 4);
    if (c.x < MAZE_SIZE - 1){
        cellList->cells[cellList->size++] = maze->cells[c.x + 1][c.y];
        API::setColor(c.x + 1, c.y, 'b');
    }
    if (c.x > 0){
        cellList->cells[cellList->size++] = maze->cells[c.x - 1][c.y];
        API::setColor(c.x - 1, c.y, 'b');
    }
    if (c.y < MAZE_SIZE - 1){
        cellList->cells[cellList->size++] = maze->cells[c.x][c.y + 1];
        API::setColor(c.x, c.y + 1, 'b');
    }
    if (c.y > 0){
        cellList->cells[cellList->size++] = maze->cells[c.x][c.y - 1];
        API::setColor(c.x, c.y - 1, 'b');
}
    return cellList;
};

void floodFill(Maze* maze, bool to_start) { // function to be called everytime you move into a new cell
    
}

int main(){
    // struct Queue* q = (Queue*)malloc(sizeof(Queue*));
    struct Maze* maze = (Maze*)malloc(sizeof(Maze));
    // initQueue(q);

    // enqueue(q, {{50, 1}, -1, -1});
    initMaze(maze);

    //printf("%i", dequeue(q).cellWalls);
    while (true) {
        scanWalls(maze);
        updateSimulator(*maze);
        if (!API::wallLeft()) {
            API::turnLeft();
            changeDir(maze, direction_index[maze->mouse_dir - 1 < 0 ? 3 : maze->mouse_dir - 1]);
            
        }
        while (API::wallFront()) {
            API::turnRight();
            changeDir(maze, direction_index[maze->mouse_dir + 1 > 3 ? 0 : maze->mouse_dir + 1]);
        }
        API::moveForward();
        updateMousePos(maze, &maze->mouse_pos, direction_index[maze->mouse_dir]);
        getNeighborCells(maze, maze->mouse_pos);
    }
}