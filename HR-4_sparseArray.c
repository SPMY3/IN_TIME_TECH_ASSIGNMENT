#include <stdio.h>
#include <stdlib.h>

//compare 2 strings logic
int stringCompare(char* string1, char* string2)
{
    while(*string1 != '\0' && *string2 != '\0')
    {
        if(*string1 != *string2)
            return 0;
        string1++;
        string2++;
    }
    return (*string1 =='\0' && *string2 == '\0');
}

//  compute count matches for each query
int* matchingStrings(int stringList_count, char** stringList, int queries_count, char** queries, int* result_count)
{
    *result_count = queries_count;
    int* result=(int*)malloc(queries_count * sizeof(int));

    for(int queriesIndex = 0; queriesIndex < queries_count; queriesIndex++)
    {
        int count=0;
        for(int stringIndex = 0; stringIndex < stringList_count; stringIndex++)
        {
            if(stringCompare(*(queries+queriesIndex), *(stringList+stringIndex)))
            {
                count++;
            }
        }
        *(result + queriesIndex)=count;
    }
    return result;
}

int main()
{
    int stringList_count, queries_count;
    int result_count;

    printf("Enter number of input strings: ");
    scanf("%d", &stringList_count);

    char** stringList = (char**)malloc(stringList_count * sizeof(char*));

    for (int stringIndex = 0; stringIndex < stringList_count; stringIndex++)
    {
        *(stringList + stringIndex) = (char*)malloc(100 * sizeof(char));
        printf("Enter string %d: ", stringIndex + 1);
        scanf("%s", *(stringList + stringIndex));
    }

    printf("\nEnter number of query strings: ");
    scanf("%d", &queries_count);

    char** queries = (char**)malloc(queries_count * sizeof(char*));

    for (int queriesIndex = 0; queriesIndex < queries_count; queriesIndex++)
    {
        *(queries + queriesIndex) = (char*)malloc(100 * sizeof(char));
        printf("Enter query %d: ", queriesIndex + 1);
        scanf("%s", *(queries + queriesIndex));
    }

    // Call matchingStrings function
    int* result = matchingStrings(stringList_count, stringList, queries_count, queries, &result_count);

    // Display results
    printf("\nQuery Results (Number of Matches):\n");
    for (int index = 0; index < result_count; index++)
    {
        printf("%d\n", *(result + index));
    }

    // Free all allocated memory
    for (int index = 0; index < stringList_count; index++)
        free(*(stringList + index));

    for (int index = 0; index < queries_count; index++)
        free(*(queries + index));

    free(stringList);
    free(queries);
    free(result);

    return 0;
}