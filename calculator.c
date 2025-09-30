#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100  // Maximum length for input expression

// Function to remove all spaces from the input string
void rmSpaces(char *s) {
    int i=0,j=0;
    while(s[i]) { 
        if(s[i]!=' ') s[j++]=s[i];  // Copy only non-space characters
        i++; 
    }
    s[j]='\0'; // Null-terminate the cleaned string
}

// Function to check if the expression contains only valid characters
// Returns 1 if valid, 0 if invalid
int valid(char *s) {
    for(int i=0;s[i];i++) {
        if(!isdigit(s[i]) && s[i]!='+' && s[i]!='-' && s[i]!='*' && s[i]!='/') 
            return 0;  // Invalid character found
    }
    return 1;
}

// Function to parse numbers and operators into separate arrays
// Returns the number of numbers found
int parse(char *s,int nums[],char ops[]) {
    int ni=0,oi=0,i=0;
    while(s[i]) {
        if(isdigit(s[i])) {
            int n=0;
            // Build the full number if it has multiple digits
            while(isdigit(s[i])) { n=n*10 + (s[i]-'0'); i++; }
            nums[ni++]=n;  // Store the number
        } else ops[oi++]=s[i++]; // Store the operator
    }
    ops[oi]='\0'; // Null-terminate the operator array
    return ni;
}

// Function to evaluate multiplication and division first (operator precedence)
// Modifies nums[] and ops[] arrays and returns the new count of numbers
int mulDiv(int nums[], char ops[], int n, int *newN) {
    int tmpN[MAX], tmpO[MAX], ni=0, oi=0;
    tmpN[ni++] = nums[0]; // Start with the first number

    for(int i=0; ops[i]; i++) {
        if(ops[i] == '*') {
            tmpN[ni-1] *= nums[i+1];  // Multiply with next number
        } else if(ops[i] == '/') {
            if(nums[i+1]==0){ 
                printf("Error: Division by zero\n"); 
                return 0; 
            }
            tmpN[ni-1] /= nums[i+1];  // Divide by next number
        } else {
            // + or - operators are saved for later
            tmpO[oi++] = ops[i];
            tmpN[ni++] = nums[i+1];  // Move next number to temp array
        }
    }

    // Copy results back to original arrays
    tmpO[oi]='\0';
    for(int i=0;i<ni;i++) nums[i]=tmpN[i];
    for(int i=0;i<oi;i++) ops[i]=tmpO[i];
    ops[oi]='\0';
    *newN = ni;
    return 1;
}

// Function to evaluate addition and subtraction
int addSub(int nums[], char ops[], int n) {
    int res=nums[0]; // Start with the first number
    for(int i=0;ops[i];i++) 
        res = (ops[i]=='+') ? res+nums[i+1] : res-nums[i+1]; // Perform addition/subtraction
    return res;
}

int main() {
    char expr[MAX];   // Input expression
    int nums[MAX], newN; // Arrays to store numbers and operators
    char ops[MAX];

    printf("Enter expression: ");
    fgets(expr,MAX,stdin); 
    expr[strcspn(expr,"\n")]='\0'; // Remove newline character

    rmSpaces(expr); // Remove spaces from input
    if(!valid(expr)) { printf("Error: Invalid expression\n"); return 0; }

    int n=parse(expr,nums,ops); // Parse input into numbers and operators
    if(!mulDiv(nums,ops,n,&newN)) return 0; // Evaluate * and / first
    printf("%d\n",addSub(nums,ops,newN)); // Evaluate + and - and print result

    return 0;
}
