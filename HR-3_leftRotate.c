#include <stdio.h>
#include <stdlib.h>

// Function to rotate an array left by 'd' positions 
int* rotateLeft(int d, int array_count, int* array, int* result_count) 
{
    *result_count = array_count;
    int* result = (int*)malloc(array_count * sizeof(int));

    d = d % array_count; 

    for (int index = 0; index < array_count; index++)
    {
        *(result + index) = *(array + ((index + d) % array_count));
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

    int* array = (int*)malloc(size * sizeof(int));

    printf("Enter %d elements:\n", size);
    for (int index = 0; index < size; index++)
    {
        scanf("%d", array + index);
    }

    int result_count;
    int* result = rotateLeft(d, size, array, &result_count);

    printf("\nArray after %d left rotations:\n", d);
    for (int index = 0; index < result_count; index++)
    {
        printf("%d ", *(result + index));
    }

    printf("\n");

    free(array);
    free(result);

    return 0;
}
