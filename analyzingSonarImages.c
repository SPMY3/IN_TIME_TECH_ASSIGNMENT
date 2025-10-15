#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 100

//generate random values for matrix
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

// rotate the matrix 90' clockwise
void rotateMatrix(int *matrix, int size)
{
    for(int layer=0;layer<size/2;layer++)
    {
        int first=layer;
        int last=size-1-layer;

        for(int i=first;i<last;i++)
        {
            int offset=i-first;

            int *top = matrix + first * size + i;
            int *left = matrix + (last - offset) * size + first;
            int *bottom = matrix + last * size + (last - offset);
            int *right = matrix + i * size + last;

            int temp=*top;
            *top=*left;
            *left=*bottom;
            *bottom=*right;
            *right=temp;

        }
    }
}

// applying smoothing filter using inplace
void smoothingFilter(int *matrix, int size)
{
    int tempRow[MAX];

    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {

            int sum=0;
            int count=0;

            for(int di=-1;di<=1;di++)
            {
                for(int dj=-1;dj<=1;dj++)
                {
                    int ni=i+di;
                    int nj=j+dj;
                    if(ni >= 0 && ni < size && nj >= 0 && nj < size)
                    {
                        sum += *(matrix + ni * size + nj);
                        count++;
                    }
                }
            }
            tempRow[j] = sum / count;
        }
        for(int j=0; j<size; j++)
        {
            *(matrix + i * size + j) = tempRow[j];
        }
    }
}

//print the matrix    
void printMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%4d", *(matrix + i * n + j));
        }
        printf("\n");
    }
}

int main()
{
    int matrix[MAX][MAX];

    int size;
    printf("Enter matrix size (2-10): ");
    scanf("%d",&size);

    if(size < 2 || size > 10)
    {
        printf("invalid size!!\n");
        return 1;
    }

    generateRandomMatrix(&matrix[0][0],size);

    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix(&matrix[0][0], size);

    rotateMatrix(&matrix[0][0], size);
    printf("\nMatrix after 90' rotation: \n");
    printMatrix(&matrix[0][0], size);

    printf("\nMatrix after smoothing filter: \n");
    smoothingFilter(&matrix[0][0], size);
    printMatrix(&matrix[0][0], size);

    return 0;
}