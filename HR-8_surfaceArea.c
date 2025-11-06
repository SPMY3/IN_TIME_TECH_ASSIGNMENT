#include <stdio.h>
#include <stdlib.h>

int surfaceArea(int heightCount, int widthCount, int** matrix) {
    int totalArea = 0;

    for (int rowIndex = 0; rowIndex < heightCount; rowIndex++) {
        for (int colIndex = 0; colIndex < widthCount; colIndex++) {
            int cubeHeight = *(*(matrix + rowIndex) + colIndex);
            if (cubeHeight > 0)
                totalArea += 6 * cubeHeight - 2 * (cubeHeight - 1);

            if (rowIndex > 0)
                totalArea -= 2 * ((*(*(matrix + rowIndex) + colIndex) < *(*(matrix + rowIndex - 1) + colIndex)) 
                                   ? *(*(matrix + rowIndex) + colIndex) 
                                   : *(*(matrix + rowIndex - 1) + colIndex));

            if (colIndex > 0)
                totalArea -= 2 * ((*(*(matrix + rowIndex) + colIndex) < *(*(matrix + rowIndex) + colIndex - 1)) 
                                   ? *(*(matrix + rowIndex) + colIndex) 
                                   : *(*(matrix + rowIndex) + colIndex - 1));
        }
    }

    return totalArea;
}

int main() {
    int heightCount, widthCount;
    scanf("%d %d", &heightCount, &widthCount);

    int** matrix = (int**)malloc(heightCount * sizeof(int*));
    for (int rowIndex = 0; rowIndex < heightCount; rowIndex++) {
        *(matrix + rowIndex) = (int*)malloc(widthCount * sizeof(int));
        for (int colIndex = 0; colIndex < widthCount; colIndex++)
            scanf("%d", (*(matrix + rowIndex) + colIndex));
    }

    int result = surfaceArea(heightCount, widthCount, matrix);
    printf("%d\n", result);

    for (int rowIndex = 0; rowIndex < heightCount; rowIndex++)
        free(*(matrix + rowIndex));
    free(matrix);

    return 0;
}
