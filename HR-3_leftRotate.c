#include <stdio.h>
#include <stdlib.h>

// Function to rotate an array left by 'd' positions 
int* rotateLeft(int d, int arr_count, int* arr, int* result_count) 
{
    *result_count = arr_count;
    int* result = (int*)malloc(arr_count * sizeof(int));

    d = d % arr_count; 

    for (int index = 0; index < arr_count; index++)
    {
        *(result + index) = *(arr + ((index + d) % arr_count));
    }

    return result;
}

int main()
{
    int size, d;

    printf("Enter size of array: ");
    scanf("%d", &size);

    printf("Enter number of left rotations: ");
    scanf("%d", &d);

    int* arr = (int*)malloc(size * sizeof(int));

    printf("Enter %d elements:\n", size);
    for (int index = 0; index < size; index++)
    {
        scanf("%d", arr + index);
    }

    int result_count;
    int* result = rotateLeft(d, size, arr, &result_count);

    printf("\nArray after %d left rotations:\n", d);
    for (int index = 0; index < result_count; index++)
    {
        printf("%d ", *(result + index));
    }

    printf("\n");

    free(arr);
    free(result);

    return 0;
}
