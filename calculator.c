#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

void rmSpaces(char *s) {
    int i=0,j=0;
    while(s[i]) { if(s[i]!=' ') s[j++]=s[i]; i++; }
    s[j]='\0';
}

int valid(char *s) {
    for(int i=0;s[i];i++) {
        if(!isdigit(s[i]) && s[i]!='+' && s[i]!='-' && s[i]!='*' && s[i]!='/') return 0;
    }
    return 1;
}

int parse(char *s,int nums[],char ops[]) {
    int ni=0,oi=0,i=0;
    while(s[i]) {
        if(isdigit(s[i])) {
            int n=0;
            while(isdigit(s[i])) { n=n*10 + (s[i]-'0'); i++; }
            nums[ni++]=n;
        } else ops[oi++]=s[i++];
    }
    ops[oi]='\0';
    return ni;
}

int mulDiv(int nums[], char ops[], int n, int *newN) {
    int tmpN[MAX], tmpO[MAX], ni=0, oi=0;
    tmpN[ni++] = nums[0];

    for(int i=0; ops[i]; i++) {
        if(ops[i] == '*') {
            tmpN[ni-1] *= nums[i+1];
        } else if(ops[i] == '/') {
            if(nums[i+1]==0){ 
                printf("Error: Division by zero\n"); 
                return 0; 
            }
            tmpN[ni-1] /= nums[i+1];
        } else {
            // + or - go to next stage
            tmpO[oi++] = ops[i];
            tmpN[ni++] = nums[i+1];
        }
    }

    tmpO[oi]='\0';
    for(int i=0;i<ni;i++) nums[i]=tmpN[i];
    for(int i=0;i<oi;i++) ops[i]=tmpO[i];
    ops[oi]='\0';
    *newN = ni;
    return 1;
}


int addSub(int nums[], char ops[], int n) {
    int res=nums[0];
    for(int i=0;ops[i];i++) res = (ops[i]=='+') ? res+nums[i+1] : res-nums[i+1];
    return res;
}

int main() {
    char expr[MAX];
    int nums[MAX], newN;
    char ops[MAX];

    printf("Enter expression: ");
    fgets(expr,MAX,stdin); expr[strcspn(expr,"\n")]='\0';

    rmSpaces(expr);
    if(!valid(expr)) { printf("Error: Invalid expression\n"); return 0; }

    int n=parse(expr,nums,ops);
    if(!mulDiv(nums,ops,n,&newN)) return 0;
    printf("%d\n",addSub(nums,ops,newN));
    return 0;
}