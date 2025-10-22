#include <stdio.h>
#include <stdlib.h>

// swap 2 numbers
void swap(int * a, int* b)
{
    int temp=0;
    temp=*a;
    *a=*b;*b=temp;
}

// sorting the customers based on serve time
void sort(int* serveTimes, int* customers,int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(*(serveTimes+j) > *(serveTimes+j+1) || (*(serveTimes+j) == *(serveTimes+j+1) && *(customers+j) > *(customers+j+1)))
            {
                swap(serveTimes+j, serveTimes+j+1);
                swap(customers+j, customers+j+1);
            }
        }
    }
}


int* jimOrders(int orders_rows, int orders_columns, int** orders, int* result_count)
{
    *result_count = orders_rows;
    
    int* result = (int*)malloc(orders_rows * sizeof(int));
    int* serveTimes = (int*)malloc(orders_rows * sizeof(int));
    int* customers = (int*)malloc(orders_rows * sizeof(int));
    
    for(int i=0;i<orders_rows;i++)
    {
        *(serveTimes+i)=*(*(orders+i)+0)+*(*(orders+i)+1);
        *(customers+i)=i+1;
    }
    
    sort(serveTimes, customers, orders_rows);
    
    for(int i=0;i<orders_rows;i++)
    {
        *(result+i)=*(customers+i);
    }
    
    free(serveTimes);
    free(customers);
    
    return result;
    
}

int main()
{
    int size;

    // Input number of customers
    printf("Enter number of customers: ");
    scanf("%d", &size);

    // Allocate memory for orders (each order has 2 integers: order number and prep time)
    int** orders = (int**)malloc(size * sizeof(int*));
    for (int index = 0; index < size; index++)
        orders[index] = (int*)malloc(2 * sizeof(int));

    // Input orders and prep times
    printf("Enter order number and preparation time for each customer:\n");
    for (int index = 0; index < size; index++)
    {
        scanf("%d %d", *(orders + index) + 0, *(orders + index) + 1);
    }

    // Call jimOrders
    int result_count;
    int* result = jimOrders(size, 2, orders, &result_count);

    // Print delivery order
    printf("\nDelivery order of customers:\n");
    for (int index = 0; index < result_count; index++)
        printf("%d ", *(result + index));

    printf("\n");

    // Free allocated memory
    for (int index = 0; index < size; index++)
        free(*(orders + index));
    free(orders);
    free(result);

    return 0;
}