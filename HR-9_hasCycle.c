#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definition of a singly linked list node
struct SinglyLinkedListNode {
    int data;
    struct SinglyLinkedListNode* next;
};

// Function to detect if a cycle exists in the linked list
bool has_cycle(struct SinglyLinkedListNode* head) 
{
    struct SinglyLinkedListNode* slowPointer = head;
    struct SinglyLinkedListNode* fastPointer = head;
    
    while (fastPointer != NULL && fastPointer->next != NULL)
    {
        slowPointer = slowPointer->next;
        fastPointer = fastPointer->next->next;

        if (slowPointer == fastPointer)
            return true;
    }

    return false;
}

// Function to create a new node
struct SinglyLinkedListNode* create_node(int value)
{
    struct SinglyLinkedListNode* newNode = (struct SinglyLinkedListNode*)malloc(sizeof(struct SinglyLinkedListNode));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
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

    printf("Enter the values of the nodes:\n");
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
    currentNode->next = head->next;

    if (has_cycle(head))
        printf("Cycle detected in the linked list.\n");
    else
        printf("No cycle detected in the linked list.\n");

    if (!has_cycle(head)) {
        struct SinglyLinkedListNode* temporaryNode;
        while (head != NULL) {
            temporaryNode = head;
            head = head->next;
            free(temporaryNode);
        }
    }

    return 0;
}
