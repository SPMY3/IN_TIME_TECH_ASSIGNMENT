#include <stdio.h>

int main() {
	
    int number,sum=0,digit;
    scanf("%d", &number);

    //Complete the code to calculate the sum of the five digits on n.
    while(number>0)
    {
        digit=number%10;
        sum+=digit;
        number=number/10;
    }
    
    printf("%d",sum);
    return 0;
}