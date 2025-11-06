#include <stdio.h>
#include <stdlib.h>

// 1. Find string length
int getStringLength(char *stringPointer) 
{
    char *currentCharPointer = stringPointer;

    while (*currentCharPointer) 
    {
        currentCharPointer++;
    }

    return currentCharPointer - stringPointer;
}

// 2. Copy string
void copyString(char *destinationPointer, char *sourcePointer) 
{
    while (*sourcePointer) 
    {
        *destinationPointer++ = *sourcePointer++;
    }

    *destinationPointer = '\0';
}

// 3. Concatenate strings
void concatenateStrings(char *destinationPointer, char *sourcePointer) 
{
    while (*destinationPointer) 
    {
        destinationPointer++; 
    }
    while (*sourcePointer) 
    {
        *destinationPointer++ = *sourcePointer++;
    }

    *destinationPointer = '\0';
}

// 4. Compare strings
int compareStrings(char *firstStringPointer, char *secondStringPointer) 
{
    while (*firstStringPointer && *secondStringPointer && *firstStringPointer == *secondStringPointer) 
    {
        firstStringPointer++;
        secondStringPointer++;
    }

    return *firstStringPointer - *secondStringPointer;
}

// 5. Reverse string
void reverseString(char *stringPointer) 
{
    char *startPointer = stringPointer;
    char *endPointer = stringPointer;
    while (*endPointer) 
    {
        endPointer++; 
    }
    endPointer--; 

    while (startPointer < endPointer)
    {
        char tempChar = *startPointer;
        *startPointer = *endPointer;
        *endPointer = tempChar;
        startPointer++;
        endPointer--;
    }
}

// 6. Convert to uppercase
void convertToUpperCase(char *stringPointer) 
{
    while (*stringPointer) 
    {
        if (*stringPointer >= 'a' && *stringPointer <= 'z') 
        {
            *stringPointer -= 32;
        }

        stringPointer++;
    }
}

// 7. Convert to lowercase
void convertToLowerCase(char *stringPointer) 
{
    while (*stringPointer) 
    {
        if (*stringPointer >= 'A' && *stringPointer <= 'Z') 
        {
            *stringPointer += 32;
        }

        stringPointer++;
    }
}

// 8. Count vowels and consonants
void countVowelsAndConsonants(char *stringPointer, int *vowelCount, int *consonantCount) 
{
    *vowelCount = 0;
    *consonantCount = 0;
    char *currentCharPointer = stringPointer;

    while (*currentCharPointer) 
    {
        char currentChar = *currentCharPointer;
        if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z'))
        {
            char lowerChar = (currentChar >= 'A' && currentChar <= 'Z') ? currentChar + 32 : currentChar;
            if (lowerChar=='a' || lowerChar=='e' || lowerChar=='i' || lowerChar=='o' || lowerChar=='u')
            {
                (*vowelCount)++;
            }
            else
            {
                (*consonantCount)++;
            }
        }
        currentCharPointer++;
    }
}

// 9. Check if palindrome
int isPalindrome(char *stringPointer) {
    char *startPointer = stringPointer;
    char *endPointer = stringPointer;
    while (*endPointer) 
    {
        endPointer++;
    }
    endPointer--;

    while (startPointer < endPointer) 
    {
        if (*startPointer != *endPointer) 
        {
            return 0;
        }
        startPointer++;
        endPointer--;
    }
    
    return 1;
}

// 10. Find a character in string
char* findCharacterInString(char *stringPointer, char searchCharacter) 
{
    while (*stringPointer) 
    {
        if (*stringPointer == searchCharacter) 
        {
            return stringPointer;
        }
        stringPointer++;
    }

    return NULL;
}

// 11. Find a substring in string
char* findSubstringInString(char *stringPointer, char *substringPointer) 
{
    if (!*substringPointer) 
    {
        return stringPointer; 
    }
    char *currentPointerInString = stringPointer;

    while (*currentPointerInString) 
    {
        char *stringComparePointer = currentPointerInString;
        char *substringComparePointer = substringPointer;

        while (*stringComparePointer && *substringComparePointer && *stringComparePointer == *substringComparePointer) 
        {
            stringComparePointer++;
            substringComparePointer++;
        }

        if (!*substringComparePointer) 
        {
            return currentPointerInString; 
        }

        currentPointerInString++;
    }

    return NULL;
}

int main() {
    char stringOne[100] = "Hello";
    char stringTwo[100] = "World";
    char copiedString[100];

    printf("Original String One: %s\n", stringOne);
    printf("Original String Two: %s\n", stringTwo);

    printf("\n1. Length of stringOne: %d\n", getStringLength(stringOne));

    copyString(copiedString, stringOne);
    printf("2. Copied stringOne into copiedString: %s\n", copiedString);

    concatenateStrings(stringOne, stringTwo);
    printf("3. Concatenate stringOne and stringTwo: %s\n", stringOne);

    printf("4. Compare stringOne and stringTwo: %d\n", compareStrings(stringOne, stringTwo));

    reverseString(stringTwo);
    printf("5. Reverse stringTwo: %s\n", stringTwo);

    convertToUpperCase(stringTwo);
    printf("6. Uppercase stringTwo: %s\n", stringTwo);

    convertToLowerCase(stringTwo);
    printf("7. Lowercase stringTwo: %s\n", stringTwo);

    int vowelCount, consonantCount;
    countVowelsAndConsonants(stringOne, &vowelCount, &consonantCount);
    printf("8. stringOne has %d vowels and %d consonants\n", vowelCount, consonantCount);

    printf("9. Is stringTwo palindrome? %s\n", isPalindrome(stringTwo) ? "Yes" : "No");

    char searchChar = 'o';
    char *foundCharPointer = findCharacterInString(stringOne, searchChar);
    if (foundCharPointer)
        printf("10. Character '%c' found at position: %ld\n", searchChar, foundCharPointer - stringOne);
    else
        printf("10. Character '%c' not found\n", searchChar);

    char *searchSubstring = "lloWo";
    char *foundSubstringPointer = findSubstringInString(stringOne, searchSubstring);
    if (foundSubstringPointer)
        printf("11. Substring '%s' found at position: %ld\n", searchSubstring, foundSubstringPointer - stringOne);
    else
        printf("11. Substring '%s' not found\n", searchSubstring);

    return 0;
}
