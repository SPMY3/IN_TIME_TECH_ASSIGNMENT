#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sortString(char *inputString) 
{
    int characterFrequency[26] = {0};
    int stringLength = strlen(inputString);

    for (int index = 0; index < stringLength; index++) 
    {
        characterFrequency[inputString[index] - 'a']++;
    }

    int writeIndex = 0;
    for (int letterIndex = 0; letterIndex < 26; letterIndex++) 
    {
        while (characterFrequency[letterIndex]-- > 0) 
        {
            inputString[writeIndex++] = 'a' + letterIndex;
        }
    }

    inputString[writeIndex] = '\0';
}

int sherlockAndAnagrams(char* inputString) 
{
    int stringLength = strlen(inputString);
    int anagramPairCount = 0;

    char **substringList = malloc(sizeof(char*) * stringLength * stringLength);
    int substringCount = 0;

    for (int startIndex = 0; startIndex < stringLength; startIndex++) 
    {
        for (int endIndex = startIndex; endIndex < stringLength; endIndex++) 
        {
            
            int substringLength = endIndex - startIndex + 1;
            char *currentSubstring = malloc(substringLength + 1);
            strncpy(currentSubstring, inputString + startIndex, substringLength);
            currentSubstring[substringLength] = '\0';
            sortString(currentSubstring);
            substringList[substringCount++] = currentSubstring;
        }
    }

    for (int firstIndex = 0; firstIndex < substringCount; firstIndex++) 
    {
        for (int secondIndex = firstIndex + 1; secondIndex < substringCount; secondIndex++) 
        {
            if (strcmp(substringList[firstIndex], substringList[secondIndex]) == 0) 
            {
                anagramPairCount++;
            }
        }
    }

    for (int index = 0; index < substringCount; index++) 
    {
        free(substringList[index]);
    }

    free(substringList);

    return anagramPairCount;
}

int main() 
{
    char inputString[1000];
    scanf("%s", inputString);

    int result = sherlockAndAnagrams(inputString);
    printf("%d\n", result);

    return 0;
}
