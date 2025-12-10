#include <stdio.h>
#include <stdlib.h>

int* total_number_of_books;
int** total_number_of_pages;

int main()
{
    int total_number_of_shelves;
    scanf("%d", &total_number_of_shelves);
    
    int total_number_of_queries;
    scanf("%d", &total_number_of_queries);
    
    total_number_of_books = (int*)calloc(total_number_of_shelves, sizeof(int));
    total_number_of_pages = (int**)calloc(total_number_of_shelves, sizeof(int*));
    for (int i = 0; i < total_number_of_shelves; i++)
        *(total_number_of_pages + i) = NULL;

    while (total_number_of_queries--) {
        int type_of_query;
        scanf("%d", &type_of_query);
        
        if (type_of_query == 1) {
            /*
             * Process the query of first type here.
             */
            int shelfIndex, numberOfPages;
            scanf("%d %d", &shelfIndex, &numberOfPages);

            int currentBookCount = *(total_number_of_books + shelfIndex);
            currentBookCount++;

            int* updatedBooks = (int*)realloc(*(total_number_of_pages + shelfIndex),
                                              currentBookCount * sizeof(int));
            if (!updatedBooks) {
                printf("Memory allocation failed!\n");
                return 1;
            }

            *(total_number_of_pages + shelfIndex) = updatedBooks;
            *(*(total_number_of_pages + shelfIndex) + currentBookCount - 1) = numberOfPages;
            *(total_number_of_books + shelfIndex) = currentBookCount;
        } 
        else if (type_of_query == 2) {
            int shelfIndex, bookIndex;
            scanf("%d %d", &shelfIndex, &bookIndex);
            printf("%d\n", *(*(total_number_of_pages + shelfIndex) + bookIndex));
        } 
        else {
            int shelfIndex;
            scanf("%d", &shelfIndex);
            printf("%d\n", *(total_number_of_books + shelfIndex));
        }
    }

    if (total_number_of_books) free(total_number_of_books);
    for (int i = 0; i < total_number_of_shelves; i++)
        if (*(total_number_of_pages + i)) free(*(total_number_of_pages + i));
    if (total_number_of_pages) free(total_number_of_pages);

    return 0;
}
