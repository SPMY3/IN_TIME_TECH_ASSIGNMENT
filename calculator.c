#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAX_EXPR_LEN 100

// Remove spaces from the expression
void removeSpaces(char *expression)
{
    int readIndex=0;
    int writeIndex=0;

    while (expression[readIndex])
    {
        if (expression[readIndex] != ' ')
        {
            expression[writeIndex++] = expression[readIndex];
        }
        readIndex++;
    }
    expression[readIndex]='\0';
}

// Check that the expression only has digits and valid operators
int isValidExpression(const char *expression)
{
    size_t length = strlen(expression);

    if (length == 0)
    {
        return 0;
    }

    if (!isdigit(expression[length - 1]))
    {
        return 0;
    }

    if (!isdigit(expression[0]))
    {
        return 0;
    }

    for(int position = 1; position < length; position++)
    {
        if (!isdigit(expression[position]) && !isdigit(expression[position-1]))
        {
            return 0;
        }

        if (!isdigit(expression[position]) &&
            expression[position] != '+' &&
            expression[position] != '-' &&
            expression[position] != '*' &&
            expression[position] != '/')
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
    int position = 0;

    while (expression[position])
    {
        if (isdigit(expression[position]))
        {
            int value = 0;
            while (isdigit(expression[position]))
            {
                value = value * 10 + (expression[position] - '0');
                position++;
            }
            numbers[numberIndex++] = value;
        }
        else
        {
            operators[operatorIndex++] = expression[position++];
        }
    }

    operators[operatorIndex] = '\0';
    return numberIndex;
}

// Handle * and / first
int handleMulDiv(int numbers[], char operators[], int count, int *updatedCount)
{
    int tempNumbers[MAX_EXPR_LEN];
    char tempOperators[MAX_EXPR_LEN];
    int numberIndex = 0;
    int operatorIndex = 0;

    tempNumbers[numberIndex++] = numbers[0];

    for (int position=0; operators[position]; position++)
    {
        if (operators[position] == '*')
        {
            tempNumbers[numberIndex - 1] *= numbers[position + 1];
        }
        else if (operators[position] == '/')
        {
            if(numbers[position + 1] == 0)
            {
                printf("Error: Division by zero\n");
                return 0;
            }
            tempNumbers[numberIndex - 1] /= numbers[position + 1];
        }

        else
        {
            tempOperators[operatorIndex++] = operators[position];
            tempNumbers[numberIndex++]=numbers[position+1];
        }
    }
    tempOperators[operatorIndex] = '\0';

    for (int position = 0; position < operatorIndex; position++)
    {
        operators[position] = tempOperators[position];    
    }

    for (int position = 0; position < numberIndex; position++)
    {
        numbers[position] = tempNumbers[position];
    }

    operators[operatorIndex] = '\0';
    *updatedCount = numberIndex;
    return 1;
}

// handle + and - using the remaining numbers and operators
int handleAddSub(const int numbers[], const char operators[], int count)
{
    int result = numbers[0];
    for (int position = 0; operators[position]; position++)
    {
        result = (operators[position] == '+') ? result + numbers[position + 1] : result - numbers[position + 1];
    }
    return result;
}

int main(void)
{
    char expression[MAX_EXPR_LEN];
    int numbers[MAX_EXPR_LEN];
    char operators[MAX_EXPR_LEN];
    int updatedCount;
    
    printf("Enter expression: ");
    fgets(expression, MAX_EXPR_LEN, stdin);
    expression[strcspn(expression, "\n")] = '\0';

    removeSpaces(expression);

    if(!isValidExpression(expression))
    {
        printf("Invalid expression\n");
        return 0;
    }

    int count = parseExpression(expression, numbers, operators);

    if(!handleMulDiv(numbers, operators, count, &updatedCount))
    {
        return 0;
    }
    printf("%d\n", handleAddSub(numbers, operators, updatedCount));
    return 0;
}
