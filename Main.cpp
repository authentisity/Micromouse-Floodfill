#include "API.h" 

#include "string.h"
#include <chrono>
#include <thread>



#define MAZE_SIZE 16
#define MAX_QUEUE_SIZE 100
#define GOAL_SIZE 4
#define NULL_CELL {{-1, -1}, NONE, 0, 0, false}

// direction
const char dir_chars[4] = {'n', 'e', 's', 'w'};

// allows you to use bitwise OR when updating which walls are present in a cell
// also allows you to bitwise AND
const int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};

enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
    NONE = 9
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

const Coord GOAL_COORDS[GOAL_SIZE] = {{(int)MAZE_SIZE / 2 - 1, (int)MAZE_SIZE / 2 - 1}, {(int)MAZE_SIZE / 2, (int)MAZE_SIZE / 2 - 1}, {(int)MAZE_SIZE / 2 - 1, (int)MAZE_SIZE / 2}, {(int)MAZE_SIZE / 2, (int)MAZE_SIZE / 2}};

struct Cell {
    Coord pos;
    Direction direction; // direction of previous node
    int cellWalls;
    int distance;
    bool discovered;
};

bool operator == (Cell c1, Cell c2){
    return c1.pos == c2.pos && c1.cellWalls == c2.cellWalls;
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
            maze->cells[x][y] = {{x, y}, NONE, 0, 0, false};
            
            // printf("%i, %i, %i, %i\n", maze->cells[x][y].pos.x, maze->cells[x][y].pos.x, maze->cells[x][y].distance, maze->cells[x][y].cellWalls);
        }
    }
} 

void resetMazeDiscovered(Maze* maze){
    for(int x = 0; x < MAZE_SIZE; x++) {
        for(int y = 0; y < MAZE_SIZE; y++) {
            maze->cells[x][y].discovered = false;
            maze->cells[x][y].distance = 0;
            maze->cells[x][y].direction = NONE;
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
            if (maze.cells[x][y].cellWalls & NORTH_MASK)
                API::setWall(x, y, 'n');
            else
                API::clearWall(x, y, 'n');
            if (maze.cells[x][y].cellWalls & EAST_MASK)
                API::setWall(x, y, 'e');
            else
                API::clearWall(x, y, 'e');
            if (maze.cells[x][y].cellWalls & SOUTH_MASK)
                API::setWall(x, y, 's');
            else
                API::clearWall(x, y, 's');
            if (maze.cells[x][y].cellWalls & WEST_MASK)
                API::setWall(x, y, 'w');
            else
                API::clearWall(x, y, 'w');
        }
    }
}
void updateMouseDir(Maze* maze, Direction dir) {
    maze->mouse_dir = dir;
}

void updateMousePos(Maze* maze) {
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
    if (c.x < MAZE_SIZE - 1 && (maze->cells[c.x][c.y].cellWalls & EAST_MASK) == 0b0000 && !maze->cells[c.x + 1][c.y].discovered){
        maze->cells[c.x + 1][c.y].discovered = true;
        maze->cells[c.x + 1][c.y].distance = maze->cells[c.x][c.y].distance + 1;
        maze->cells[c.x + 1][c.y].direction = WEST;
        cellList->cells[cellList->size++] = maze->cells[c.x + 1][c.y];
        // API::setColor(c.x + 1, c.y, 'b');
    }
    // else
    //     API::setColor(c.x + 1, c.y, 'k');
    if (c.x > 0 && (maze->cells[c.x][c.y].cellWalls & WEST_MASK) == 0b0000 && !maze->cells[c.x - 1][c.y].discovered){
        maze->cells[c.x - 1][c.y].discovered = true;
        maze->cells[c.x - 1][c.y].distance = maze->cells[c.x][c.y].distance + 1;
        maze->cells[c.x - 1][c.y].direction = EAST;
        cellList->cells[cellList->size++] = maze->cells[c.x - 1][c.y];
        // API::setColor(c.x - 1, c.y, 'b');
    }
    // else
    //     API::setColor(c.x - 1, c.y, 'k');
    if (c.y < MAZE_SIZE - 1 && (maze->cells[c.x][c.y].cellWalls & NORTH_MASK) == 0b0000 && !maze->cells[c.x][c.y + 1].discovered){
        maze->cells[c.x][c.y + 1].discovered = true;
        maze->cells[c.x][c.y + 1].distance = maze->cells[c.x][c.y].distance + 1;
        maze->cells[c.x][c.y + 1].direction = SOUTH;
        cellList->cells[cellList->size++] = maze->cells[c.x][c.y + 1];
        // API::setColor(c.x, c.y + 1, 'b');
    }
    // else
    //     API::setColor(c.x, c.y + 1, 'k');
    if (c.y > 0 && (maze->cells[c.x][c.y].cellWalls & SOUTH_MASK) == 0b0000 && !maze->cells[c.x][c.y - 1].discovered){
        maze->cells[c.x][c.y - 1].discovered = true;
        maze->cells[c.x][c.y - 1].distance = maze->cells[c.x][c.y].distance + 1;
        maze->cells[c.x][c.y - 1].direction = NORTH;
        cellList->cells[cellList->size++] = maze->cells[c.x][c.y - 1];
        // API::setColor(c.x, c.y - 1, 'b');
    }
    // else
    //     API::setColor(c.x, c.y - 1, 'k');
    return cellList;
};

void floodFill(Maze* maze, Queue* q, bool to_start) { // function to be called everytime you move into a new cell
    enqueue(q, maze->cells[maze->mouse_pos.x][maze->mouse_pos.y]);
    maze->cells[maze->mouse_pos.x][maze->mouse_pos.y].discovered = true;
    Cell path_node = NULL_CELL; 

    do {
        CellList* list = getNeighborCells(maze, dequeue(q).pos);
        // add each neighboring cell thats not blocked by a wall or already searched into the queue
        for (int i = 0; i < list->size; i++){
            for (int j = 0; j < GOAL_SIZE; j++){
                if (list->cells[i].pos == GOAL_COORDS[j]){
                    path_node = maze->cells[list->cells[i].pos.x][list->cells[i].pos.y];
                    // API::setColor(list->cells[i].pos.x, list->cells[i].pos.y, 'g');
                    goto path_found;
                }
            }
            enqueue(q, list->cells[i]);
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        // sleep(1);
        // for(int x = 0; x < MAZE_SIZE; x++) {
        //     for(int y = 0; y < MAZE_SIZE; y++) {
        //         API::setColor(x, y, 'k');
        //     }
        // }
    } while(!isQEmpty(q));

    path_found:

    for(int x = 0; x < MAZE_SIZE; x++) 
    {
        for(int y = 0; y < MAZE_SIZE; y++) 
        {
            API::setColor(x, y, 'k');
            API::setText(x, y, std::string(1, (char)(maze->cells[x][y].direction + '0')));
        }
    }

    while(!(path_node.pos == maze->mouse_pos)){
    //     API::setText(
    // std::string{static_cast<char>(path_node.pos.x + '0'), ',', static_cast<char>(path_node.pos.y + '0'), '-', static_cast<char>(maze->mouse_pos.x + '0'), ',', static_cast<char>(maze->mouse_pos.y + '0')});
        API::setColor(path_node.pos.x, path_node.pos.y, 'g');
        if (path_node.direction == NORTH)
            path_node = maze->cells[path_node.pos.x][path_node.pos.y + 1];
        else if (path_node.direction == SOUTH)
            path_node = maze->cells[path_node.pos.x][path_node.pos.y - 1];
        else if (path_node.direction == EAST)
            path_node = maze->cells[path_node.pos.x + 1][path_node.pos.y];
        else if (path_node.direction == WEST)
            path_node = maze->cells[path_node.pos.x - 1][path_node.pos.y];
    }

    // for(int x = 0; x < MAZE_SIZE; x++) {
    //     for(int y = 0; y < MAZE_SIZE; y++) {
    //         API::setText(x, y, std::string(1, (char)(maze->cells[x][y].distance + '0')));
    //     }
    // }
    resetMazeDiscovered(maze);
    initQueue(q);
}

int main(){
    struct Queue* q = (Queue*)malloc(sizeof(Queue));
    struct Maze* maze = (Maze*)malloc(sizeof(Maze));
    initQueue(q);
    initMaze(maze);

    while (true) {
        scanWalls(maze);
        updateSimulator(*maze);
        floodFill(maze, q, false);
        if (!API::wallLeft()) {
            API::turnLeft();
            updateMouseDir(maze, direction_index[maze->mouse_dir - 1 < 0 ? 3 : maze->mouse_dir - 1]);
            
        }
        while (API::wallFront()) {
            API::turnRight();
            updateMouseDir(maze, direction_index[maze->mouse_dir + 1 > 3 ? 0 : maze->mouse_dir + 1]);
        }
        API::moveForward();
        updateMousePos(maze);
    }
}