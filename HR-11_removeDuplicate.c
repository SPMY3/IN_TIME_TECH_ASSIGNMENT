#include <stdio.h>
#include <stdlib.h>

// Definition of a singly linked list node
struct SinglyLinkedListNode {
    int data;
    struct SinglyLinkedListNode* next;
};

// Function to remove duplicate elements from a sorted linked list
struct SinglyLinkedListNode* removeDuplicates(struct SinglyLinkedListNode* head) 
{
    struct SinglyLinkedListNode* currentNode = head;
    struct SinglyLinkedListNode* duplicateNode;

    while (currentNode != NULL && currentNode->next != NULL)
    {
        if (currentNode->data == currentNode->next->data)
        {
            duplicateNode = currentNode->next;
            currentNode->next = currentNode->next->next;
            free(duplicateNode);
        }
        else 
        {
            currentNode = currentNode->next;
        }
    }
    return head;
}

// Function to create a new node
struct SinglyLinkedListNode* create_node(int value)
{
    struct SinglyLinkedListNode* newNode = (struct SinglyLinkedListNode*)malloc(sizeof(struct SinglyLinkedListNode));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// Function to print the linked list
void print_linked_list(struct SinglyLinkedListNode* head)
{
    struct SinglyLinkedListNode* currentNode = head;
    while (currentNode != NULL)
    {
        printf("%d ", currentNode->data);
        currentNode = currentNode->next;
    }
    printf("\n");
}

int main()
{
    int numberOfNodes;
    printf("Enter the number of nodes in the linked list: ");
    scanf("%d", &numberOfNodes);

    if (numberOfNodes <= 0) {
        printf("The number of nodes must be greater than zero.\n");
        return 0;
    }

    struct SinglyLinkedListNode* head = NULL;
    struct SinglyLinkedListNode* currentNode = NULL;

    printf("Enter the values of the nodes (in sorted order):\n");
    for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++)
    {
        int nodeValue;
        scanf("%d", &nodeValue);

        struct SinglyLinkedListNode* newNode = create_node(nodeValue);

        if (head == NULL)
            head = newNode;
        else
            currentNode->next = newNode;

        currentNode = newNode;
    }

    printf("Original linked list:\n");
    print_linked_list(head);

    head = removeDuplicates(head);

    printf("Linked list after removing duplicates:\n");
    print_linked_list(head);

    // Free memory
    struct SinglyLinkedListNode* temporaryNode;
    while (head != NULL)
    {
        temporaryNode = head;
        head = head->next;
        free(temporaryNode);
    }

    return 0;
}
