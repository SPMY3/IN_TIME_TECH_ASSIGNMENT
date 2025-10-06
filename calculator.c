#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100
// Remove spaces from the expression so we can parse it easily
void removeSpaces(char *s) {
    int i = 0, j = 0;
    while (s[i]) { 
        if (s[i] != ' ') s[j++] = s[i];  
        i++; 
    }
    s[j] = '\0'; 
}

// Check that the expression only has digits and valid operators
int valid(char *s) {
    int len = strlen(s);
    if (len == 0) return 0;
    if (!isdigit(s[0]) && s[0] != '-') return 0;
    if (!isdigit(s[len - 1])) return 0;
    for (int i = 1; i < len; i++) {
        if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/')
            return 0;
        if (!isdigit(s[i]) && !isdigit(s[i-1]))
            return 0;
    }
    return 1;
}

// Split the input into numbers and operators
int parse(char *s, int nums[], char ops[]) {
    int ni = 0, oi = 0, i = 0;
    while (s[i]) {
        if (isdigit(s[i])) {
            int n = 0;
            while (isdigit(s[i])) {
                n = n * 10 + (s[i] - '0');
                i++; 
            }
            nums[ni++] = n;
        } else {
            ops[oi++] = s[i++];
        }
    }
    ops[oi] = '\0';
    return ni;
}

// Handle * and / first before + and - 
int mulDiv(int nums[], char ops[], int n, int *newN) {
    int tempN[MAX], tempO[MAX], ni = 0, oi = 0;
    tempN[ni++] = nums[0];

    for (int i = 0; ops[i]; i++) {
        if (ops[i] == '*') {
            tempN[ni - 1] *= nums[i + 1];
        } else if (ops[i] == '/') {
            if (nums[i + 1] == 0) { 
                printf("Error: Division by zero\n");
                return 0; 
            }
            tempN[ni - 1] /= nums[i + 1];  
        } else {
            tempO[oi++] = ops[i];
            tempN[ni++] = nums[i + 1];
        }
    }

    tempO[oi] = '\0';
    for (int i = 0; i < ni; i++) nums[i] = tempN[i];
    for (int i = 0; i < oi; i++) ops[i] = tempO[i];
    ops[oi] = '\0';
    *newN = ni;
    return 1;
}

// handle + and - using the remaining numbers and operators
int addSub(int nums[], char ops[], int n) {
    int res = nums[0];
    for (int i = 0; ops[i]; i++) 
        res = (ops[i] == '+') ? res + nums[i + 1] : res - nums[i + 1]; 
    return res;
}

int main() {
    char expr[MAX];   
    int nums[MAX], newN; 
    char ops[MAX];
    printf("Enter expression: ");
    fgets(expr, MAX, stdin); 
    expr[strcspn(expr, "\n")] = '\0';
    removeSpaces(expr); 
    if (!valid(expr)) {
        printf("Error: Invalid expression\n");
        return 0;
    }
    int n = parse(expr, nums, ops);
    if (!mulDiv(nums, ops, n, &newN)) 
        return 0;
    printf("%d\n", addSub(nums, ops, newN));
    return 0;
}

