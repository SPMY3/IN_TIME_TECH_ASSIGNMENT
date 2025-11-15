#include <stdio.h>
#include <stdlib.h>

int minimumMoves(int gridSize, char** grid, int startRow, int startCol, int goalRow, int goalCol) 
{
    int size = gridSize;

    int **distanceMatrix = malloc(size * sizeof(int *));
    for (int row = 0; row < size; row++) 
    {
        distanceMatrix[row] = malloc(size * sizeof(int));
        for (int col = 0; col < size; col++) 
        {
            distanceMatrix[row][col] = -1;  
        }
    }

    typedef struct { int row; int col; } Cell;
    Cell *queue = malloc(size * size * sizeof(Cell));
    int queueFrontIndex = 0;
    int queueBackIndex = 0;

    distanceMatrix[startRow][startCol] = 0;
    queue[queueBackIndex++] = (Cell){startRow, startCol};

    int movementDirections[4][2] = {
        {1, 0},  
        {-1, 0}, 
        {0, 1},  
        {0, -1}  
    };

    while (queueFrontIndex < queueBackIndex) 
    {
        Cell currentCell = queue[queueFrontIndex++];
        int currentRow = currentCell.row;
        int currentCol = currentCell.col;

        if (currentRow == goalRow && currentCol == goalCol) 
        {
            int result = distanceMatrix[currentRow][currentCol];

            for (int row = 0; row < size; row++) 
            {
                free(distanceMatrix[row]);
            }

            free(distanceMatrix);
            free(queue);

            return result;
        }

        for (int directionIndex = 0; directionIndex < 4; directionIndex++) 
        {
            int deltaRow = movementDirections[directionIndex][0];
            int deltaCol = movementDirections[directionIndex][1];

            int nextRow = currentRow + deltaRow;
            int nextCol = currentCol + deltaCol;

            while (nextRow >= 0 && nextRow < size &&
                   nextCol >= 0 && nextCol < size &&
                   grid[nextRow][nextCol] == '.') 
            {
                if (distanceMatrix[nextRow][nextCol] == -1) 
                {
                    distanceMatrix[nextRow][nextCol] = distanceMatrix[currentRow][currentCol] + 1;
                    queue[queueBackIndex++] = (Cell){nextRow, nextCol};
                }

                nextRow += deltaRow;
                nextCol += deltaCol;
            }
        }
    }

    return -1;
}

int main() 
{
    int gridSize;
    scanf("%d", &gridSize);

    char **grid = malloc(gridSize * sizeof(char *));
    for (int row = 0; row < gridSize; row++) 
    {
        grid[row] = malloc((gridSize + 1) * sizeof(char));
        scanf("%s", grid[row]);
    }

    int startRow, startCol, goalRow, goalCol;
    scanf("%d %d %d %d", &startRow, &startCol, &goalRow, &goalCol);

    int result = minimumMoves(gridSize, grid, startRow, startCol, goalRow, goalCol);
    printf("%d\n", result);

    for (int row = 0; row < gridSize; row++) 
    {
        free(grid[row]);
    }
    free(grid);

    return 0;
}
