#include <stdio.h>
#include <stdlib.h>

int main()
{
    int number, *arr, i;
    scanf("%d", &number);
    arr = (int*) malloc(number * sizeof(int));
    for(int index = 0; index < number; index++) {
        scanf("%d", arr + index);
    }


    /* Write the logic to reverse the array. */
    int *start = arr, *end = arr + number - 1, temp;
    while(start < end)
    {
        temp = *start;
        *start = *end;
        *end = temp;
        
        start++;
        end--;
    }

    for(int index = 0; index < number; index++)
        printf("%d ", *(arr + index));
    return 0;
}