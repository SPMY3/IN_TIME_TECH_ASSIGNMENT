#include <stdio.h>
#include <stdlib.h>

//structure to store array of products
typedef struct 
{
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

//print the product details
void printProduct(Product *productPointer) 
{
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           productPointer->id, productPointer->name, productPointer->price, productPointer->quantity);
}

//get input of product details
Product inputProductDetails() 
{
    Product newProduct;
    char tempName[50]; 

    printf("Product ID: ");
    scanf("%d", &newProduct.id);
    getchar(); 

    printf("Product Name: ");
    int inputChar, nameIndex = 0;
    while ((inputChar = getchar()) != '\n' && inputChar != EOF && nameIndex < 49)
    {
        *(tempName + nameIndex) = inputChar;
        nameIndex++;
    }
    *(tempName + nameIndex) = '\0';

    char *sourceChar = tempName;
    char *destinationChar = newProduct.name;
    while (*sourceChar)
        *destinationChar++ = *sourceChar++;
    *destinationChar = '\0';               

    printf("Product Price: ");
    scanf("%f", &newProduct.price);

    printf("Product Quantity: ");
    scanf("%d", &newProduct.quantity);

    return newProduct;
}

//1. Add New Product 
void addProduct(Product **productListPointer, int *numberOfProducts) 
{
    (*numberOfProducts)++;
    Product *resizedProductList = realloc(*productListPointer, (*numberOfProducts) * sizeof(Product));
    if (!resizedProductList) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    *productListPointer = resizedProductList;

    printf("\nEnter details for new product:\n");
    Product *newProductPointer = *productListPointer + (*numberOfProducts - 1);
    *newProductPointer = inputProductDetails();
}
//2. View All Products 
void viewProducts(Product *productListPointer, int numberOfProducts) 
{
    Product *endPointer = productListPointer + numberOfProducts;
    for (Product *currentProductPointer = productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        printProduct(currentProductPointer);
    }
}

//3. Update Quantity 
void updateQuantity(Product *productListPointer, int numberOfProducts) 
{
    int searchId, found = 0;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &searchId);

    Product *endPointer = productListPointer + numberOfProducts;
    for (Product *currentProductPointer = productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        if (currentProductPointer->id == searchId)
        {
            printf("Enter new Quantity: ");
            scanf("%d", &currentProductPointer->quantity);
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found!\n");
}

//4. Search Product by ID 
void searchByID(Product *productListPointer, int numberOfProducts) 
{
    int searchId, found = 0;
    printf("Enter the Product ID to search: ");
    scanf("%d", &searchId);

    Product *endPointer = productListPointer + numberOfProducts;
    for (Product *currentProductPointer = productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        if (currentProductPointer->id == searchId)
        {
            printProduct(currentProductPointer);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found!\n");
}

char *partialStringCompare(const char *productNamePointer, const char *searchNamePointer) 
{
    if (*searchNamePointer == '\0') 
        return (char *)productNamePointer;

    const char *currentProductCharPointer = productNamePointer;
    while (*currentProductCharPointer)
    {
        const char *productCharPointer = currentProductCharPointer;
        const char *searchCharPointer = searchNamePointer;

        while (*productCharPointer && *searchCharPointer && *productCharPointer == *searchCharPointer)
        {
            productCharPointer++;
            searchCharPointer++;
        }

        if (*searchCharPointer == '\0')
            return (char *)currentProductCharPointer;

        currentProductCharPointer++;
    }

    return NULL;
}

//5. Search Product by Name 
void searchByName(Product *productListPointer, int numberOfProducts) 
{
    char searchName[50];
    printf("Enter name to search (partial allowed): ");
    getchar();
    int inputChar, searchIndex = 0;
    while ((inputChar = getchar()) != '\n' && inputChar != EOF && searchIndex < 49)
    {
        *(searchName + searchIndex) = inputChar;
        searchIndex++;
    }
    *(searchName + searchIndex) = '\0';

    int found = 0;
    Product *endPointer = productListPointer + numberOfProducts;
    for (Product *currentProductPointer = productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        if (partialStringCompare(currentProductPointer->name, searchName))
        {
            printProduct(currentProductPointer);
            found = 1;
        }
    }

    if (!found)
        printf("No products found matching '%s'\n", searchName);
}

//6. Search Product by Price Range 
void searchByPriceRange(Product *productListPointer, int numberOfProducts) 
{
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    int found = 0;
    Product *endPointer = productListPointer + numberOfProducts;
    for (Product *currentProductPointer = productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        if (currentProductPointer->price >= minPrice && currentProductPointer->price <= maxPrice)
        {
            printProduct(currentProductPointer);
            found = 1;
        }
    }

    if (!found)
        printf("No products found in this price range.\n");
}

//7. Delete Product 
void deleteProduct(Product **productListPointer, int *numberOfProducts) 
{
    int searchId, found = 0;
    printf("Enter Product ID to delete: ");
    scanf("%d", &searchId);

    Product *endPointer = *productListPointer + *numberOfProducts;
    for (Product *currentProductPointer = *productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        if (currentProductPointer->id == searchId)
        {
            found = 1;
            Product *nextProductPointer = currentProductPointer + 1;
            while (nextProductPointer < endPointer)
            {
                *(nextProductPointer - 1) = *nextProductPointer;
                nextProductPointer++;
            }

            (*numberOfProducts)--;
            if (*numberOfProducts > 0)
            {
                Product *resizedProductList = realloc(*productListPointer, (*numberOfProducts) * sizeof(Product));
                if (resizedProductList)
                    *productListPointer = resizedProductList;
            }
            else
            {
                free(*productListPointer);
                *productListPointer = NULL;
            }

            printf("Product deleted successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Product not found!\n");
}

//8. Exit - freeing dynamically allocated memory
void freeMemory(Product *productListPointer) 
{
    free(productListPointer);
}

int main() 
{
    int choice, numberOfProducts;

    printf("Enter initial number of products: ");
    scanf("%d", &numberOfProducts);
    getchar();

    Product *productListPointer = (Product *)calloc(numberOfProducts, sizeof(Product));
    if (!productListPointer) 
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    Product *endPointer = productListPointer + numberOfProducts;
    for (Product *currentProductPointer = productListPointer; currentProductPointer < endPointer; currentProductPointer++)
    {
        printf("\nEnter details for product:\n");
        *currentProductPointer = inputProductDetails();
    }

    do // INVENTORY MENU 
    {
        printf("========= INVENTORY MENU =========");
        printf("\n1. Add New Product\n"
               "2. View All Products\n"
               "3. Update Quantity\n"
               "4. Search Product by ID\n"
               "5. Search Product by Name\n"
               "6. Search Product by Price Range\n"
               "7. Delete Product\n"
               "8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) 
        {
            case 1: addProduct(&productListPointer, &numberOfProducts); break;
            case 2: viewProducts(productListPointer, numberOfProducts); break;
            case 3: updateQuantity(productListPointer, numberOfProducts); break;
            case 4: searchByID(productListPointer, numberOfProducts); break;
            case 5: searchByName(productListPointer, numberOfProducts); break;
            case 6: searchByPriceRange(productListPointer, numberOfProducts); break;
            case 7: deleteProduct(&productListPointer, &numberOfProducts); break;
            case 8:
                freeMemory(productListPointer);
                printf("Memory released successfully. Exiting program.\n");
                break;
            default: printf("Enter a valid choice!\n");
        }
    } while (choice != 8);

    return 0;
}
