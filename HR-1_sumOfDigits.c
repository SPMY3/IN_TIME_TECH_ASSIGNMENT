#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
	
    int num,sum=0,digit;
    scanf("%d", &num);
    //Complete the code to calculate the sum of the five digits on n.
    while(num>0)
    {
        digit=num%10;
        sum+=digit;
        num=num/10;
    }
    printf("%d",sum);
    return 0;
}