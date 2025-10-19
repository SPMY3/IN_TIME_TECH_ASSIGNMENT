#include<stdio.h>
#include<stdlib.h>
#include<time.h>


//generate random values for matrix.
void generateRandomMatrix(int *matrix, int size)
{
    srand(time(0));
    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
            *(matrix + row * size + col) = rand() % 256;
        }
    }
    
}

// rotate the matrix 90º clockwise.
void rotateMatrix(int *matrix, int size)
{
    for(int layer=0; layer < size/2; layer++)
    {
        int first=layer;
        int last=size-1-layer;

        for(int index = first; index < last; index++)
        {
            int offset=index-first;

            int *top = matrix + first * size + index;
            int *left = matrix + (last - offset) * size + first;
            int *bottom = matrix + last * size + (last - offset);
            int *right = matrix + index * size + last;

            int temp=*top;
            *top=*left;
            *left=*bottom;
            *bottom=*right;
            *right=temp;

        }
    }
}

// applying smoothing filter using inplace.
void smoothingFilter(int *matrix, int size)
{
    int *tempRow = (int *)malloc(size * sizeof(int));
    if (!tempRow)
    {
        printf("Memory allocation failed for tempRow!\n");
        exit(1);
    }

    for(int row = 0; row < size; row++)
    {
        for(int col = 0; col < size; col++)
        {

            int sum = 0;
            int count = 0;

            for(int rowOffset = -1; rowOffset <= 1; rowOffset++)
            {
                for(int colOffset = -1; colOffset <= 1; colOffset++)
                {
                    int neighborRow = row + rowOffset;
                    int neighborCol = col + colOffset;
                    if(neighborRow >= 0 && neighborRow < size && neighborCol >= 0 && neighborCol < size)
                    {
                        sum += *(matrix + neighborRow * size + neighborCol);
                        count++;
                    }
                }
            }
            *(tempRow + col) = sum / count;
        }
        for(int index = 0;index < size; index++)
        {
            *(matrix + row * size + index) = *(tempRow + index);
        }
    }
}

//print the matrix.    
void printMatrix(int *matrix, int size) {
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%4d", *(matrix + row * size + col));
        }
        printf("\n");
    }
}

int main()
{

    int size;
    printf("Enter matrix size (2-10): ");
    scanf("%d",&size);

    if(size < 2 || size > 10)
    {
        printf("invalid size!!\n");
        return 1;
    }

    //using flat 2D array for simple pointer arithmetic, contiguous memory allocation, and efficient in-place operations.

    int *matrix = (int *)malloc(size * size * sizeof(int));
    if (!matrix) 
    {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    generateRandomMatrix(matrix,size);

    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix(matrix, size);

    rotateMatrix(matrix, size);
    printf("\nMatrix after 90º rotation: \n");
    printMatrix(matrix, size);

    printf("\nMatrix after smoothing filter: \n");
    smoothingFilter(matrix, size);
    printMatrix(matrix, size);

    return 0;
}
