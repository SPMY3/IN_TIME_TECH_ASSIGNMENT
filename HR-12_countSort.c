#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void countSort(int arr_rows, int arr_columns, char*** arr) 
{
    int max = 0;

    for (int index = 0; index < arr_rows; index++) 
    {
        int key = atoi(arr[index][0]);
        if (key > max)
        {
            max = key;
        }
    }

    char*** buckets = malloc((max + 1) * sizeof(char**));
    int* bucketSizes = calloc(max + 1, sizeof(int));

    for (int index = 0; index < arr_rows; index++) 
    {
        int key = atoi(arr[index][0]);
        bucketSizes[key]++;
    }

    for (int index = 0; index <= max; index++) 
    {
        buckets[index] = malloc(bucketSizes[index] * sizeof(char*));
        bucketSizes[index] = 0;
    }

    for (int index = 0; index < arr_rows; index++) 
    {
        int key = atoi(arr[index][0]);
        char* value = (index < arr_rows / 2) ? "-" : arr[index][1];
        buckets[key][bucketSizes[key]++] = value;
    }

    for (int index = 0; index <= max; index++) 
    {
        for (int indexj = 0; indexj < bucketSizes[index]; indexj++) 
        {
            printf("%s ", buckets[index][indexj]);
        }
    }

    printf("\n");

    for (int index = 0; index <= max; index++) 
    {
        free(buckets[index]);
    }
    free(buckets);
    free(bucketSizes);
}

int main() 
{
    int size;
    scanf("%d", &size);

    char*** array = malloc(size * sizeof(char**));

    for (int index = 0; index < size; index++) 
    {
        array[index] = malloc(2 * sizeof(char*));
        array[index][0] = malloc(10);
        array[index][1] = malloc(100);

        scanf("%s %s", array[index][0], array[index][1]);
    }

    countSort(size, 2, array);

    for (int index = 0; index < size; index++) 
    {
        free(array[index][0]);
        free(array[index][1]);
        free(array[index]);
    }
    free(array);

    return 0;
}
