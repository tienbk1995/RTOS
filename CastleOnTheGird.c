#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);

// Point structure
typedef struct point
{
    int x; // Row
    int y; // Column
} point;

// Enqueue Process (Re-entrancy)
void enQueue (point **queue, int *rear, int NodeX, int NodeY)
{
    // Add a node into the queue
    queue[*rear]->x = NodeX;
    queue[*rear]->y = NodeY;
    // Increase the tail of the queue
    *rear += 1;
}

// Dequeue Process (Re-entrancy)
point *deQueue (point **queue, int *front, int *rear)
{
    // Checking if the queue is empty
    if (*front == *rear)
    {
        return NULL;
    }
    // Define temporary pointer to use as return value
    point *temp = calloc(1, sizeof(point));
    temp->x = queue[*front]->x;
    temp->y = queue[*front]->y;
    // Remove(dequeue) the node from head of the queue
    queue[*front]->x = -1;
    queue[*front]->y = -1;
    // Increase head of the queue
    *front += 1;
    // Return the node has been de-queued
    return temp;
}

// Peak from Front of Queue
point *peekFront (point **queue, int *front, int *rear)
{
    // Checking if the queue is empty
    if (*front == *rear)
    {
        return NULL;
    }
    // Define temporary pointer to use as return value
    point *temp = malloc(sizeof(point));
    temp = queue[*front];
    // Return the node has been de-queued
    return temp;
}
// Print all current processed array
void printPoints (point **InArr, int *index)
{
    int i = 0;
    for (; i < *index; i++)
    {
        printf("Node[%d], InArr->x = %d, InArr->y = %d\n",i, InArr[i]->x, InArr[i]->y);
    }
}

// Processing the adjacent nodes of the processed node.
void listOfAdjacentNodes (char **grid, int grid_count, int processedNodeX, int processedNodeY, int *retIndx, point **retArr)
{
    // Define the current node
    int currentX = processedNodeX;
    int currentY = processedNodeY;
    int index = 0;

    // Scaning abjacent nodes of X (X -> 0)
    while (currentX > 0)
    {
        // Processing the below processed node (row)
        currentX -= 1;
        // Encounter "X" block
        if (grid[currentX][currentY] == 'X')
        {
            // Stop processing
            break;
        }
        // Assign the current adjacent nodes into "listOfAdjacentNodes"
        retArr[index]->x = currentX;
        retArr[index]->y = currentY;
        // Increase the current adjacent nodes
        index += 1;
    }
    // Store back value of processed node of X preparing for above node
    currentX = processedNodeX;
    // Scaning abjacent nodes of X (N-1 -> X)
    while (currentX < grid_count - 1)
    {
        // Processing the above processed node (row)
        currentX += 1;
        // Encounter "X" block
        if (grid[currentX][currentY] == 'X')
        {
            // Stop processing
            break;
        }
        // Assign the current adjacent nodes into "listOfAdjacentNodes"    
        retArr[index]->x = currentX;
        retArr[index]->y = currentY;
        // Increase the current adjacent nodes
        index += 1;
    }
    // Store back value o`f processed node X
    currentX = processedNodeX;

    // Scaning abjacent nodes of Y (Y -> 0)
    while (currentY > 0)
    {
        // Processing the below processed node (column)
        currentY -= 1;
        // Encounter "X" block
        if (grid[currentX][currentY] == 'X')
        {
            // Stop processing
            break;
        }
        // Assign the current adjacent nodes into "listOfAdjacentNodes"    
        retArr[index]->x = currentX;
        retArr[index]->y = currentY;
        // Increase the current adjacent nodes
        index += 1;
    }
    // Store back value of processed node Y
    currentY = processedNodeY;
    // Scaning abjacent nodes of Y (N-1 -> Y)
    while (currentY < grid_count - 1)
    {
        // Processing the above processed node (column)
        currentY += 1;
        // Encounter "X" block
        if (grid[currentX][currentY] == 'X')
        {
            // Stop processing
            break;
        }
        // Assign the current adjacent nodes into "listOfAdjacentNodes"    
        retArr[index]->x = currentX;
        retArr[index]->y = currentY;
        // Increase the current adjacent nodes
        index += 1;
    }
    // Store back value of processed node Y
    currentY = processedNodeY;
    // Return current index in processed array (total processed abjacent nodes + 1)
    *retIndx = index;
}

// Complete the minimumMoves function below.
int minimumMoves(int grid_count, char** grid, int startX, int startY, int goalX, int goalY) 
{
    // Initialize front and rear of queue
    int front = 0;
    int rear = 0;
    // Initialize the Queue
    point **Queue = calloc(grid_count*grid_count, sizeof(point*));
    for (int i = 0; i < grid_count*grid_count; i++)
    {
        Queue[i] = calloc(1, sizeof(point));
    }
    // Initialize Start-Node's distance
    grid[startX][startY] = 0;
    // Enqueue Start-Node into Queue
    enQueue(Queue, &rear, startX, startY);
    // Loop till the queue is empty (reaching shortest distance from Start-Node to Goal-Node)
    while (peekFront(Queue, &front, &rear) != NULL)
    {
        // Initialize index
        int retIndx = 0;
        // Initialize the processed Array
        point **retArr = calloc(grid_count*grid_count, sizeof(point*));
        for (int i = 0; i < grid_count*grid_count; i++)
        {
            retArr[i] = calloc(1, sizeof(point));
        }
        // Dequeue the current processed node
        point *currProcessedNode = deQueue(Queue, &front, &rear);
        // Update distance value for current processed Node
        int currDistance = grid[currProcessedNode->x][currProcessedNode->y];
        // Array stores abjacent nodes that would be updated the distance
        listOfAdjacentNodes(grid, grid_count, currProcessedNode->x, currProcessedNode->y, 
                                        &retIndx, retArr);
        // Update distance for abjacent nodes
        for (int i = 0; i < retIndx; i++)
        {
            // If node is "."
            if (grid[retArr[i]->x][retArr[i]->y] == '.')
            {
                // Update distance for this node
                grid[retArr[i]->x][retArr[i]->y] = currDistance + 1;
                // En queue this node into Queue
                enQueue(Queue, &rear, retArr[i]->x, retArr[i]->y);
                // Return if this node is Goal-Node
                if ((retArr[i]->x == goalX) && (retArr[i]->y == goalY))
                {
                    return currDistance + 1;
                }
            }
        }
    }
    // If no have any node reaches the Goal-Node
    return -1;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* n_endptr;
    char* n_str = readline();
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** grid = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* grid_item = readline();

        *(grid + i) = grid_item;
    }

    int grid_count = n;

    char** startXStartY = split_string(readline());

    char* startX_endptr;
    char* startX_str = startXStartY[0];
    int startX = strtol(startX_str, &startX_endptr, 10);

    if (startX_endptr == startX_str || *startX_endptr != '\0') { exit(EXIT_FAILURE); }

    char* startY_endptr;
    char* startY_str = startXStartY[1];
    int startY = strtol(startY_str, &startY_endptr, 10);

    if (startY_endptr == startY_str || *startY_endptr != '\0') { exit(EXIT_FAILURE); }

    char* goalX_endptr;
    char* goalX_str = startXStartY[2];
    int goalX = strtol(goalX_str, &goalX_endptr, 10);

    if (goalX_endptr == goalX_str || *goalX_endptr != '\0') { exit(EXIT_FAILURE); }

    char* goalY_endptr;
    char* goalY_str = startXStartY[3];
    int goalY = strtol(goalY_str, &goalY_endptr, 10);

    if (goalY_endptr == goalY_str || *goalY_endptr != '\0') { exit(EXIT_FAILURE); }

    int result = minimumMoves(grid_count, grid, startX, startY, goalX, goalY);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
