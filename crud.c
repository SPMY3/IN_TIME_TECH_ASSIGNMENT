#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_LEN 100

// Structure for storing user details
struct User
{
    int id;
    char name[50];
    int age;
};

// adding a new user to the file
void addUser(void)
{
    struct User user;
    FILE *filePtr = fopen("users.txt", "a");

    if (filePtr == NULL)
    {
        printf("Cannot open the file\n");
        return;
    }

    printf("Enter the ID: ");
    scanf("%d", &user.id);
    getchar();

    printf("Enter the Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Enter the Age: ");
    scanf("%d", &user.age);

    fprintf(filePtr, "%d %s %d\n", user.id, user.name, user.age);
    fclose(filePtr);

    printf("User added!\n");
}

// displays all users from the file
void showUsers(void)
{
    struct User user;
    FILE *filePtr = fopen("users.txt", "r");

    if (filePtr == NULL)
    {
        printf("No users found\n");
        return;
    }

    printf("ID\tName\tAge\n");

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) != EOF)
    {
        printf("%d\t%s\t%d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
}

// update a user by ID
void updateUser(void)
{
    int id;
    int found = 0;
    struct User user;

    FILE *filePtr = fopen("users.txt", "r");
    FILE *tempPtr = fopen("temp.txt", "w");

    if (filePtr == NULL || tempPtr == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d", &id);
    getchar();

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) != EOF)
    {
        if (user.id == id)
        {
            found = 1;

            printf("Enter new Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';

            printf("Enter new Age: ");
            scanf("%d", &user.age);
            getchar();
        }

        fprintf(tempPtr, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
    fclose(tempPtr);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    printf(found ? "Updated!\n" : "User not found\n");
}

// delete a user by ID
void deleteUser(void)
{
    int id;
    int found = 0;
    struct User user;

    FILE *filePtr = fopen("users.txt", "r");
    FILE *tempPtr = fopen("temp.txt", "w");

    if (filePtr == NULL || tempPtr == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) != EOF)
    {
        if (user.id == id)
        {
            found = 1;
            continue;
        }

        fprintf(tempPtr, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
    fclose(tempPtr);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    printf(found ? "Deleted!\n" : "User not found\n");
}

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n1.Add 2.Show 3.Update 4.Delete 5.Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            addUser();
        }
        else if (choice == 2)
        {
            showUsers();
        }
        else if (choice == 3)
        {
            updateUser();
        }
        else if (choice == 4)
        {
            deleteUser();
        }
        else if (choice == 5)
        {
            break;
        }
        else
        {
            printf("Invalid choice!!\n");
        }
    }

    return 0;
}
