#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// Remove spaces from the expression so we can parse it easily
void removeSpaces(char *expression)
{
    int i = 0;
    int j = 0;

    while (expression[i])
    {
        if (expression[i] != ' ')
        {
            expression[j++] = expression[i];
        }
        i++;
    }
    expression[j] = '\0';
}

// Check that the expression only has digits and valid operators
int isValidExpression(const char *expression)
{
    int length = strlen(expression);

    if (length == 0)
    {
        return 0;
    }

    if (!isdigit(expression[0]) && expression[0] != '-')
    {
        return 0;
    }

    if (!isdigit(expression[length - 1]))
    {
        return 0;
    }

    for (int i = 1; i < length; i++)
    {
        if (!isdigit(expression[i]) &&
            expression[i] != '+' &&
            expression[i] != '-' &&
            expression[i] != '*' &&
            expression[i] != '/')
        {
            return 0;
        }

        if (!isdigit(expression[i]) && !isdigit(expression[i - 1]))
        {
            return 0;
        }
    }
    return 1;
}

// Split the input into numbers and operators array
int parseExpression(const char *expression, int numbers[], char operators[])
{
    int numberIndex = 0;
    int operatorIndex = 0;
    int i = 0;

    while (expression[i])
    {
        if (isdigit(expression[i]))
        {
            int value = 0;
            while (isdigit(expression[i]))
            {
                value = value * 10 + (expression[i] - '0');
                i++;
            }
            numbers[numberIndex++] = value;
        }
        else
        {
            operators[operatorIndex++] = expression[i++];
        }
    }

    operators[operatorIndex] = '\0';
    return numberIndex;
}

// Handle * and / first before + and -
int handleMulDiv(int numbers[], char operators[], int count, int *updatedCount)
{
    int tempNumbers[MAX];
    char tempOperators[MAX];
    int numberIndex = 0;
    int operatorIndex = 0;

    tempNumbers[numberIndex++] = numbers[0];

    for (int i = 0; operators[i]; i++)
    {
        if (operators[i] == '*')
        {
            tempNumbers[numberIndex - 1] *= numbers[i + 1];
        }
        else if (operators[i] == '/')
        {
            if (numbers[i + 1] == 0)
            {
                printf("Error: Division by zero\n");
                return 0;
            }
            tempNumbers[numberIndex - 1] /= numbers[i + 1];
        }
        else
        {
            tempOperators[operatorIndex++] = operators[i];
            tempNumbers[numberIndex++] = numbers[i + 1];
        }
    }

    tempOperators[operatorIndex] = '\0';

    for (int i = 0; i < numberIndex; i++)
    {
        numbers[i] = tempNumbers[i];
    }

    for (int i = 0; i < operatorIndex; i++)
    {
        operators[i] = tempOperators[i];
    }

    operators[operatorIndex] = '\0';
    *updatedCount = numberIndex;
    return 1;
}

// handle + and - using the remaining numbers and operators
int handleAddSub(const int numbers[], const char operators[], int count)
{
    int result = numbers[0];
    for (int i = 0; operators[i]; i++)
    {
        result = (operators[i] == '+') ? result + numbers[i + 1] : result - numbers[i + 1];
    }
    return result;
}

int main(void)
{
    char expression[MAX];
    int numbers[MAX];
    int updatedCount;
    char operators[MAX];

    printf("Enter expression: ");
    fgets(expression, MAX, stdin);
    expression[strcspn(expression, "\n")] = '\0';

    removeSpaces(expression);

    if (!isValidExpression(expression))
    {
        printf("Error: Invalid expression\n");
        return 0;
    }

    int count = parseExpression(expression, numbers, operators);

    if (!handleMulDiv(numbers, operators, count, &updatedCount))
    {
        return 0;
    }

    printf("%d\n", handleAddSub(numbers, operators, updatedCount));
    return 0;
}
