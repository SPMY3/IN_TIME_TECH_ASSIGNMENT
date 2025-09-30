#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100  // Maximum size for strings

// Structure to store user information
struct User { 
    int id; 
    char name[50]; 
    int age; 
};

// Function to add a new user to the file
void addUser() {
    struct User u; 
    FILE *f = fopen("users.txt","a"); // Open file in append mode
    if(!f){ 
        printf("Cannot open file\n"); 
        return; 
    }

    // Input user details
    printf("Enter ID: "); scanf("%d",&u.id); getchar();
    printf("Enter Name: "); fgets(u.name,50,stdin); u.name[strlen(u.name)-1]='\0';
    printf("Enter Age: "); scanf("%d",&u.age);

    // Write user info to file
    fprintf(f,"%d %s %d\n",u.id,u.name,u.age);
    fclose(f); 
    printf("User added!\n");
}

// Function to display all users from the file
void showUsers() {
    struct User u; 
    FILE *f=fopen("users.txt","r"); // Open file in read mode
    if(!f){ 
        printf("No users found\n"); 
        return; 
    }

    printf("ID\tName\tAge\n");
    while(fscanf(f,"%d %s %d\n",&u.id,u.name,&u.age)!=EOF) 
        printf("%d\t%s\t%d\n",u.id,u.name,u.age);
    fclose(f);
}

// Function to update a user by ID
void updateUser() {
    int id, found=0; 
    struct User u;
    FILE *f=fopen("users.txt","r"), *t=fopen("temp.txt","w"); // Temp file for updates
    if(!f || !t){ 
        printf("Error opening file\n"); 
        return; 
    }

    printf("Enter ID to update: "); scanf("%d",&id); getchar();
    while(fscanf(f,"%d %s %d\n",&u.id,u.name,&u.age)!=EOF){
        if(u.id == id){ 
            found = 1; 
            // Input new details
            printf("Enter new Name: "); fgets(u.name,50,stdin); u.name[strlen(u.name)-1]='\0';
            printf("Enter new Age: "); scanf("%d",&u.age); getchar();
        }
        // Write to temp file
        fprintf(t,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(f); fclose(t);
    remove("users.txt"); // Replace original file with updated temp file
    rename("temp.txt","users.txt");
    printf(found ? "Updated!\n" : "User not found\n");
}

// Function to delete a user by ID
void deleteUser() {
    int id, found=0; 
    struct User u;
    FILE *f=fopen("users.txt","r"), *t=fopen("temp.txt","w");
    if(!f || !t){ 
        printf("Error opening file\n"); 
        return; 
    }

    printf("Enter ID to delete: "); scanf("%d",&id);
    while(fscanf(f,"%d %s %d\n",&u.id,u.name,&u.age)!=EOF){
        if(u.id == id){ 
            found = 1; 
            continue; // Skip writing this user to temp file
        }
        fprintf(t,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(f); fclose(t);
    remove("users.txt"); // Replace original file with updated temp file
    rename("temp.txt","users.txt");
    printf(found ? "Deleted!\n" : "User not found\n");
}

// Main function to provide a menu-driven interface
int main() {
    int ch;
    while(1){
        printf("\n1.Add 2.Show 3.Update 4.Delete 5.Exit\nChoice: ");
        scanf("%d",&ch); getchar();
        if(ch==1) addUser();
        else if(ch==2) showUsers();
        else if(ch==3) updateUser();
        else if(ch==4) deleteUser();
        else if(ch==5) break;
        else printf("Invalid\n");
    }
    return 0;
}
