#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_HASH_SIZE 1000

typedef struct DoublyLinkedNode
{
    int key;
    char data[100];
    struct DoublyLinkedNode* previous;
    struct DoublyLinkedNode* next;
} DoublyLinkedNode;

typedef struct DoublyLinkedQueue
{
    DoublyLinkedNode* head;
    DoublyLinkedNode* tail;
    int currentSize;
} DoublyLinkedQueue;

typedef struct HashTableNode
{
    int key;
    DoublyLinkedNode* linkedListNode;
    struct HashTableNode* next;
} HashTableNode;

HashTableNode* hashTable[MAX_HASH_SIZE];

int cacheMaximumCapacity = 0;
int cacheCurrentSize = 0;
DoublyLinkedQueue* cacheQueue = NULL;

//helper functions
int getHashIndex(int key)
{
    return key % MAX_HASH_SIZE;
}

void clearInputBuffer()
{
    int characterRead;
    while ((characterRead = getchar()) != '\n' && characterRead != EOF);
}

// Queue and Node Creation 
DoublyLinkedQueue* createQueue()
{
    DoublyLinkedQueue* newQueue = (DoublyLinkedQueue*)malloc(sizeof(DoublyLinkedQueue));

    if (!newQueue)
    {
        printf("Memory allocation failed!");
        exit(1);
    }

    newQueue->head = NULL;
    newQueue->tail = NULL;
    newQueue->currentSize = 0;
    return newQueue;
}

DoublyLinkedNode* createDoublyLinkedNode(int key, const char* data)
{
    DoublyLinkedNode* newNode = (DoublyLinkedNode*)malloc(sizeof(DoublyLinkedNode));
    if (!newNode)
    {
        printf("Memory allocation failed!");
        exit(1);
    }

    newNode->key = key;
    strncpy(newNode->data, data, sizeof(newNode->data) - 1);
    newNode->data[sizeof(newNode->data) - 1] = '\0';
    newNode->previous = NULL;
    newNode->next = NULL;

    return newNode;
}

//Hash Table Operations
void insertIntoHashTable(int key, DoublyLinkedNode* node)
{
    int index = getHashIndex(key);
    HashTableNode* currentNode = hashTable[index];

    while (currentNode != NULL)
    {
        if (currentNode->key == key)
        {
            currentNode->linkedListNode = node;
            return;
        }
        currentNode = currentNode->next;
    }

    HashTableNode* newHashNode = (HashTableNode*)malloc(sizeof(HashTableNode));
    newHashNode->key = key;
    newHashNode->linkedListNode = node;
    newHashNode->next = hashTable[index];
    hashTable[index] = newHashNode;
}

DoublyLinkedNode* getFromHashTable(int key)
{
    int index = getHashIndex(key);
    HashTableNode* currentNode = hashTable[index];

    while (currentNode != NULL)
    {
        if (currentNode->key == key)
        {
            return currentNode->linkedListNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

void deleteFromHashTable(int key)
{
    int index = getHashIndex(key);
    HashTableNode* currentNode = hashTable[index];
    HashTableNode* previousNode = NULL;

    while (currentNode != NULL)
    {
        if (currentNode->key == key)
        {
            if (previousNode == NULL)
            {
                hashTable[index] = currentNode->next;
            }
            else
            {
                previousNode->next = currentNode->next;
            }
            free(currentNode);
            return;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
}

//Queue Operations for LRU Cache
void addNodeToFront(DoublyLinkedQueue* queue, DoublyLinkedNode* node)
{
    node->previous = NULL;
    node->next = queue->head;

    if (queue->head != NULL)
    {
        queue->head->previous = node;
    }

    queue->head = node;

    if (queue->tail == NULL)
    {
        queue->tail = node;
    }

    queue->currentSize++;
}

void removeNodeFromQueue(DoublyLinkedQueue* queue, DoublyLinkedNode* node)
{
    if (node->previous != NULL)
    {
        node->previous->next = node->next;
    }
    else
    {
        queue->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->previous = node->previous;
    }
    else
    {
        queue->tail = node->previous;
    }

    node->next = NULL;
    node->previous = NULL;
    queue->currentSize--;
}

DoublyLinkedNode* removeLastNode(DoublyLinkedQueue* queue)
{
    if (queue->tail == NULL)
    {
        return NULL;
    }

    DoublyLinkedNode* lastNode = queue->tail;
    queue->tail = lastNode->previous;

    if (queue->tail != NULL)
    {
        queue->tail->next = NULL;
    }
    else
    {
        queue->head = NULL;
    }

    lastNode->previous = NULL;
    queue->currentSize--;

    return lastNode;
}

//LRU Cache Operations
void initializeCache(int capacity)
{
    cacheQueue = createQueue();
    cacheMaximumCapacity = capacity;
    cacheCurrentSize = 0;

    for (int index = 0; index < MAX_HASH_SIZE; index++)
    {
        hashTable[index] = NULL;
    }
}

void putIntoCache(int key, const char* data)
{
    DoublyLinkedNode* existingNode = getFromHashTable(key);

    if (existingNode != NULL)
    {
        strncpy(existingNode->data, data, sizeof(existingNode->data) - 1);
        existingNode->data[sizeof(existingNode->data) - 1] = '\0';

        removeNodeFromQueue(cacheQueue, existingNode);
        addNodeToFront(cacheQueue, existingNode);
        return;
    }

    if (cacheCurrentSize == cacheMaximumCapacity)
    {
        DoublyLinkedNode* leastRecentlyUsedNode = removeLastNode(cacheQueue);
        deleteFromHashTable(leastRecentlyUsedNode->key);
        free(leastRecentlyUsedNode);
        cacheCurrentSize--;
    }

    DoublyLinkedNode* newNode = createDoublyLinkedNode(key, data);
    addNodeToFront(cacheQueue, newNode);
    insertIntoHashTable(key, newNode);
    cacheCurrentSize++;
}

char* getFromCache(int key)
{
    DoublyLinkedNode* node = getFromHashTable(key);

    if (node == NULL)
        return "NULL";

    removeNodeFromQueue(cacheQueue, node);
    addNodeToFront(cacheQueue, node);

    return node->data;
}

//Input Validation Functions
int readCacheCapacity()
{
    char userInput[100];

    while (1)
    {
        scanf(" %s", userInput);
        clearInputBuffer();

        int isValidNumber = 1;

        for (int index = 0; userInput[index] != '\0'; index++)
        {
            if (!isdigit(userInput[index]))
            {
                isValidNumber = 0;
                break;
            }
        }

        if (!isValidNumber)
        {
            printf("Invalid. Enter only numbers: ");
            continue;
        }

        int capacityValue = atoi(userInput);
        if (capacityValue < 0 || capacityValue > MAX_HASH_SIZE)
        {
            printf("Out of range (0–999). Enter again: ");
            continue;
        }

        return capacityValue;
    }
}

int readKeyInput()
{
    char userInput[100];

    while (1)
    {
        scanf(" %s", userInput);
        clearInputBuffer();

        int isValidNumber = 1;

        for (int index = 0; userInput[index] != '\0'; index++)
        {
            if (!isdigit(userInput[index]))
            {
                isValidNumber = 0;
                break;
            }
        }

        if (!isValidNumber)
        {
            printf("Invalid. Enter only numbers: ");
            continue;
        }

        int keyValue = atoi(userInput);

        if (keyValue <= 0)
        {
            printf("Invalid. Enter a number greater than 0: ");
            continue;
        }

        return keyValue;
    }
}

void readLine(char* buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int isValidCommandName(const char* command)
{
    if (command == NULL || strlen(command) == 0)
    {
        return 0;
    }

    for (int index = 0; command[index] != '\0'; index++)
    {
        if (!isalpha(command[index]))
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int userKeyInput;
    char userDataInput[50];
    char userCommand[50];

    while (1)
    {
        printf("Enter option (createCache): ");
        readLine(userCommand, sizeof(userCommand));

        if (!isValidCommandName(userCommand))
        {
            printf("Invalid. Create Cache first using \"createCache\"\n");
            continue;
        }

        if (strcmp(userCommand, "createCache") == 0)
        {
            printf("Enter cache capacity: ");
            int capacity = readCacheCapacity();
            initializeCache(capacity);
            break;
        }

        printf("Invalid. Create Cache first using \"createCache\"\n");
    }
    
    while (1)
    {
        printf("Enter option (put/get/exit): ");
        readLine(userCommand, sizeof(userCommand));

        if (!isValidCommandName(userCommand))
        {
            printf("Invalid\n");
            continue;
        }

        if (strcmp(userCommand, "put") == 0)
        {
            printf("Enter key: ");
            userKeyInput = readKeyInput();

            printf("Enter data: ");
            scanf(" %s", userDataInput);
            clearInputBuffer();

            putIntoCache(userKeyInput, userDataInput);
        }
        else if (strcmp(userCommand, "get") == 0)
        {
            printf("Enter key: ");
            userKeyInput = readKeyInput();

            char* result = getFromCache(userKeyInput);
            printf("%s\n", result);
        }
        else if (strcmp(userCommand, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid\n");
        }
    }

    return 0;
}
