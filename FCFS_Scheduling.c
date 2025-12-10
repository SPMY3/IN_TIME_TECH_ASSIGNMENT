#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ENUM: PROCESS STATES
typedef enum 
{
    STATE_NEW,
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED,
    STATE_KILLED
} ProcessState;

typedef struct PCB 
{
    char processName[64];
    int processId;
    int cpuBurstTime;
    int ioStartTime;
    int ioDuration;

    int cpuExecutedTime;
    int cpuRemainingTime;
    int ioRemainingTime;
    int ioAlreadyStarted;

    int arrivalTime;
    int completionTime;
    ProcessState state;

    struct PCB *nextInQueue;
    struct PCB *nextInHash;
} PCB;

typedef struct Queue 
{
    PCB *front;
    PCB *rear;
} Queue;

typedef struct KillEvent 
{
    int killPid;
    int killTime;
    struct KillEvent *next;
} KillEvent;

PCB *processHashTable[97];
Queue readyQueue, waitingQueue, terminatedQueue;
KillEvent *killEventList = NULL;

int totalProcessCount = 0;
int totalTerminatedCount = 0;

// Queue Function
//Initializes an empty queue 
void initializeQueue(Queue *queue) 
{
    queue->front = NULL;
    queue->rear = NULL;
}

//Adds a process to the end of a queue 
void enqueueProcess(Queue *queue, PCB *process) 
{
    process->nextInQueue = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = process;
    } else {
        queue->rear->nextInQueue = process;
        queue->rear = process;
    }
}

//Removes and returns the first process in a queue
PCB* dequeueProcess(Queue *queue) 
{
    if (queue->front == NULL)
        return NULL;

    PCB *process = queue->front;
    queue->front = process->nextInQueue;

    if (queue->front == NULL)
        queue->rear = NULL;

    process->nextInQueue = NULL;
    return process;
}

//Removes a specific process (by PID) from a queue
int removeProcessFromQueue(Queue *queue, int pid) 
{
    PCB *previous = NULL, *current = queue->front;

    while (current != NULL) 
    {
        if (current->processId == pid) 
        {
            if (previous != NULL)
                previous->nextInQueue = current->nextInQueue;
            else
                queue->front = current->nextInQueue;

            if (current == queue->rear)
                queue->rear = previous;

            current->nextInQueue = NULL;
            return 1;
        }
        previous = current;
        current = current->nextInQueue;
    }
    return 0;
}

//Returns hash value (index) for a PID
int hashFunction(int pid) 
{
    if (pid < 0) pid = -pid;
    return pid % 97;
}

//Inserts a PCB into the hash table
void insertIntoHashTable(PCB *process) 
{
    int index = hashFunction(process->processId);
    process->nextInHash = processHashTable[index];
    processHashTable[index] = process;
}

//Retrieves a PCB from the hash table using PID
PCB* getProcessFromHash(int pid) 
{
    int index = hashFunction(pid);
    PCB *current = processHashTable[index];

    while (current != NULL) 
    {
        if (current->processId == pid)
            return current;
        current = current->nextInHash;
    }
    return NULL;
}

// Creates and initializes a new PCB structure
PCB* createPCB(char *name, int pid, int cpu, int ioStart, int ioDur) 
{
    PCB *process = (PCB*) malloc(sizeof(PCB));

    strcpy(process->processName, name);
    process->processId = pid;
    process->cpuBurstTime = cpu;
    process->ioStartTime = ioStart;
    process->ioDuration = ioDur;

    process->cpuExecutedTime = 0;
    process->cpuRemainingTime = cpu;
    process->ioRemainingTime = ioDur;
    process->ioAlreadyStarted = 0;

    process->arrivalTime = 0;
    process->completionTime = -1;
    process->state = STATE_READY;

    process->nextInQueue = NULL;
    process->nextInHash = NULL;

    return process;
}

// Adds a kill event to the sorted kill-event list
void addKillEvent(int pid, int time) 
{
    KillEvent *event = (KillEvent*) malloc(sizeof(KillEvent));
    event->killPid = pid;
    event->killTime = time;
    event->next = NULL;

    if (killEventList == NULL || killEventList->killTime > time) 
    {
        event->next = killEventList;
        killEventList = event;
        return;
    }

    KillEvent *current = killEventList;
    while (current->next != NULL && current->next->killTime <= time)
        current = current->next;

    event->next = current->next;
    current->next = event;
}

//Returns and removes all kill events scheduled for the current time
KillEvent* getKillEventsAtTime(int time) 
{
    KillEvent *result = NULL, *last = NULL;

    while (killEventList != NULL && killEventList->killTime == time) 
    {
        KillEvent *event = killEventList;
        killEventList = killEventList->next;

        event->next = NULL;

        if (result == NULL)
            result = last = event;
        else 
        {
            last->next = event;
            last = event;
        }
    }

    return result;
}

//Marks a process as terminated or killed and moves it to terminated queue
void terminateProcess(PCB *process, int time, ProcessState state) 
{
    process->completionTime = time;
    process->state = state;

    enqueueProcess(&terminatedQueue, process);
    totalTerminatedCount++;
}

//I/O WAITING QUEUE UPDATE
void updateWaitingProcesses(int currentTime) 
{
    PCB *previous = NULL;
    PCB *current = waitingQueue.front;

    while (current != NULL) 
    {
        PCB *next = current->nextInQueue;
        current->ioRemainingTime--;

        if (current->ioRemainingTime <= 0) 
        {
            if (previous != NULL)
                previous->nextInQueue = next;
            else
                waitingQueue.front = next;

            if (current == waitingQueue.rear)
                waitingQueue.rear = previous;

            current->state = STATE_READY;
            current->ioAlreadyStarted = 0;
            current->nextInQueue = NULL;

            enqueueProcess(&readyQueue, current);

            printf("[t=%d] IO complete → PID %d → READY\n",
                   currentTime, current->processId);
        } 
        else 
        {
            previous = current;
        }

        current = next;
    }
}

//Assigns a READY process to the CPU if CPU is idle
void dispatchProcess(int currentTime, PCB **runningProcess) {
    if (*runningProcess == NULL) {
        PCB *process = dequeueProcess(&readyQueue);

        if (process != NULL) {
            process->state = STATE_RUNNING;
            *runningProcess = process;

            printf("[t=%d] DISPATCH → PID %d is RUNNING\n",
                   currentTime, process->processId);
        }
    }
}

//Executes one CPU cycle for the running process
void executeOneTimeUnit(int currentTime, PCB **runningProcess) 
{
    if (*runningProcess == NULL) 
    {
        printf("[t=%d] CPU IDLE\n", currentTime);
        return;
    }

    PCB *process = *runningProcess;

    process->cpuExecutedTime++;
    process->cpuRemainingTime--;

    printf("[t=%d] RUNNING PID %d (Executed %d/%d)\n",
           currentTime, process->processId,
           process->cpuExecutedTime, process->cpuBurstTime);

    if (process->ioStartTime >= 0 &&
        process->ioAlreadyStarted == 0 &&
        process->cpuExecutedTime == process->ioStartTime)
    {
        process->state = STATE_WAITING;
        process->ioRemainingTime = process->ioDuration;
        process->ioAlreadyStarted = 1;

        enqueueProcess(&waitingQueue, process);

        printf("[t=%d] BLOCKED → PID %d moved to WAITING\n",
               currentTime, process->processId);

        *runningProcess = NULL;
        return;
    }

    if (process->cpuRemainingTime <= 0) 
    {
        terminateProcess(process, currentTime + 1, STATE_TERMINATED);
        *runningProcess = NULL;
    }
}

//Applies kill events at the current time and terminates matching processes 
void applyKillEvents(int currentTime, PCB **runningProcess) 
{
    KillEvent *events = getKillEventsAtTime(currentTime);

    while (events != NULL) 
    {
        PCB *target = getProcessFromHash(events->killPid);

        if (target != NULL &&
            target->state != STATE_TERMINATED &&
            target->state != STATE_KILLED)
        {
            if (*runningProcess != NULL &&
                (*runningProcess)->processId == target->processId)
            {
                printf("[t=%d] KILL → Running PID %d KILLED\n",
                       currentTime, target->processId);

                terminateProcess(target, currentTime, STATE_KILLED);
                *runningProcess = NULL;
            } 
            else 
            {
                removeProcessFromQueue(&readyQueue, target->processId);
                removeProcessFromQueue(&waitingQueue, target->processId);

                printf("[t=%d] KILL → PID %d terminated\n",
                       currentTime, target->processId);

                terminateProcess(target, currentTime, STATE_KILLED);
            }
        }

        KillEvent *temp = events;
        events = events->next;
        free(temp);
    }
}

//Reads process details from user and loads them into ready queue
void readProcessInput() 
{
    printf("Enter number of processes: ");
    scanf("%d", &totalProcessCount);

    for (int index = 0; index < totalProcessCount; index++) 
    {
        char name[64];
        int pid, cpu, ioStart, ioDuration;

        printf("Process %d (name pid cpu ioStart ioDuration): ",
               index + 1);

        scanf("%s %d %d %d %d",
              name, &pid, &cpu, &ioStart, &ioDuration);

        if (ioStart < 0) 
        {
            ioStart = -1;
            ioDuration = 0;
        }

        PCB *process = createPCB(name, pid, cpu, ioStart, ioDuration);
        insertIntoHashTable(process);
        enqueueProcess(&readyQueue, process);
    }
}

//Reads all kill events from user input
void readKillEvents() 
{
    int killCount;
    printf("Enter number of kill events: ");
    scanf("%d", &killCount);

    for (int index = 0; index < killCount; index++) 
    {
        int pid, time;
        printf("Kill %d (pid time): ", index + 1);
        scanf("%d %d", &pid, &time);
        addKillEvent(pid, time);
    }
}

//Runs the entire FCFS time-unit simulation loop
void runSimulation() 
{
    printf("\n=== FCFS SIMULATION STARTED ===\n\n");

    int currentTime = 0;
    PCB *runningProcess = NULL;

    while (totalTerminatedCount < totalProcessCount) 
    {
        applyKillEvents(currentTime, &runningProcess);
        updateWaitingProcesses(currentTime);
        dispatchProcess(currentTime, &runningProcess);
        executeOneTimeUnit(currentTime, &runningProcess);

        currentTime++;
    }
}

//Prints turnaround time, waiting time, and status for all processes
void printFinalResults() 
{
    printf("\n==================== FINAL OUTPUT ====================\n");
    printf("%-6s %-12s %-6s %-6s %-10s %-10s %-10s\n",
           "PID", "Name", "CPU", "IO", "TAT", "WAIT", "STATUS");

    PCB *current = terminatedQueue.front;

    while (current != NULL) 
    {
        if (current->state == STATE_KILLED) 
        {
            printf("%-6d %-12s %-6d %-6d %-10s %-10s %-10s\n",
                   current->processId, current->processName,
                   current->cpuBurstTime, current->ioDuration,
                   "-", "-", "KILLED");
        } 
        else 
        {
            int tat = current->completionTime - current->arrivalTime;
            int wait = tat - (current->cpuBurstTime + current->ioDuration);
            if (wait < 0) wait = 0;

            printf("%-6d %-12s %-6d %-6d %-10d %-10d %-10s\n",
                   current->processId, current->processName,
                   current->cpuBurstTime, current->ioDuration,
                   tat, wait, "OK");
        }

        current = current->nextInQueue;
    }
}

int main() 
{

    for (int index = 0; index < 97; index++)
        processHashTable[index] = NULL;

    initializeQueue(&readyQueue);
    initializeQueue(&waitingQueue);
    initializeQueue(&terminatedQueue);

    readProcessInput();
    readKillEvents();
    runSimulation();
    printFinalResults();

    return 0;
}
