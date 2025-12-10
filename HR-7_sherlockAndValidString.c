#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* isValid(char* str) 
{
    static char yes[] = "YES";
    static char no[]  = "NO";

    int strLength = strlen(str);
    int *frequency = (int*)calloc(26, sizeof(int));

    for (int index = 0; index < strLength; index++)
        (*(frequency + (str[index] - 'a')))++;

    int mismatchCount = 0;
    int firstNonZeroIndex = 0, secondNonZeroIndex = -100;

    for (int index = 0; index < 26; index++) 
    {
        if (*(frequency + index)) {
            firstNonZeroIndex = index;
            break;
        }
    }

    for (int index = firstNonZeroIndex + 1; index < 26; index++) 
    {
        if (*(frequency + index)) {
            secondNonZeroIndex = index;
            break;
        }
    }

    if (secondNonZeroIndex == -100) {
        free(frequency);
        return yes;
    }

    for (int index = firstNonZeroIndex + 1; index < 26; index++) 
    {
        if (*(frequency + index) && 
            ((*(frequency + index) - *(frequency + firstNonZeroIndex) == 1) || (*(frequency + index) == 1)))
            mismatchCount++;
        else if (*(frequency + index) && 
                 (abs(*(frequency + index) - *(frequency + firstNonZeroIndex)) > 1 ||
                  (*(frequency + index) - *(frequency + firstNonZeroIndex) == -1)))
            mismatchCount = 2;

        if (mismatchCount == 2)
            break;
    }

    if (mismatchCount < 2) {
        free(frequency);
        return yes;
    }

    mismatchCount = 0;
    for (int index = 0; index < 26; index++) 
    {
        if (*(frequency + index) && 
            ((*(frequency + index) - *(frequency + secondNonZeroIndex) == 1) || (*(frequency + index) == 1)))
            mismatchCount++;
        else if (*(frequency + index) && 
                 (abs(*(frequency + index) - *(frequency + secondNonZeroIndex)) > 1 ||
                  (*(frequency + index) - *(frequency + secondNonZeroIndex) == -1)))
            mismatchCount = 2;

        if (mismatchCount == 2)
            break;
    }

    free(frequency);

    if (mismatchCount < 2)
        return yes;
    else
        return no;
}

int main() 
{
    char *inputString = (char*)malloc(100001 * sizeof(char));

    scanf("%s", inputString);

    printf("%s\n", isValid(inputString));

    free(inputString);
    return 0;
}
