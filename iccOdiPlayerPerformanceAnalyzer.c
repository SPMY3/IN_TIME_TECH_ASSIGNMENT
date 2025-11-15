#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ID 1500
#define MAX_NAME_LENGTH 51
#define MAX_ROLE_LENGTH 20
#define MAX_TEAM_NAME_LENGTH 50

#include "Players_data.h"

// Structure to maintain the player
typedef struct PlayerNode 
{
    int playerId;
    char playerName[MAX_NAME_LENGTH];
    char teamName[MAX_TEAM_NAME_LENGTH];
    char role[MAX_ROLE_LENGTH];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wicketsTaken;
    float economyRate;
    float performanceIndex;
    struct PlayerNode* next;
} PlayerNode;

// Structure to maintain the team
typedef struct Team 
{
    int teamId;
    char teamName[MAX_TEAM_NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikeRate;
    PlayerNode* playerList;
} Team;

Team* allTeams = NULL;
int totalTeams = 0;

// Calculates a player's performance index based on their role
float calculatePerformanceIndex(const PlayerNode* player) 
{
    if (strcmp(player->role, "Batsman") == 0) 
    {
        return (player->battingAverage * player->strikeRate) / 100.0f;
    } 
    else if (strcmp(player->role, "Bowler") == 0) 
    {
        return (player->wicketsTaken * 2.0f) + (100.0f - player->economyRate);
    } 
    else 
    {
        return ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wicketsTaken * 2.0f);
    }
}

// Compares two strings without case sensitivity
int compareStringsCaseInsensitive(const char* stringA, const char* stringB) 
{
    while (*stringA && *stringB) 
    {
        char charA = tolower((unsigned char)*stringA);
        char charB = tolower((unsigned char)*stringB);
        if (charA != charB) 
        {
            return (charA < charB) ? -1 : 1;
        }
        stringA++;
        stringB++;
    }
    if (*stringA) 
    {
        return 1;
    }
    if (*stringB) 
    {
        return -1;
    }
    return 0;
}

// Creates and returns a new PlayerNode with the provided player details
PlayerNode* createPlayerNode(int playerId, const char* playerName, const char* teamName, const char* role, int totalRuns,
    float battingAverage, float strikeRate, int wicketsTaken, float economyRate) 
{
    PlayerNode* newPlayer = (PlayerNode*)malloc(sizeof(PlayerNode));
    if (!newPlayer) 
    {
        fprintf(stderr, "Memory allocation failed for new player\n");
        exit(EXIT_FAILURE);
    }
    newPlayer->playerId = playerId;
    strncpy(newPlayer->playerName, playerName, MAX_NAME_LENGTH - 1);
    newPlayer->playerName[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(newPlayer->teamName, teamName, MAX_TEAM_NAME_LENGTH - 1);
    newPlayer->teamName[MAX_TEAM_NAME_LENGTH - 1] = '\0';
    strncpy(newPlayer->role, role, MAX_ROLE_LENGTH - 1);
    newPlayer->role[MAX_ROLE_LENGTH - 1] = '\0';
    newPlayer->totalRuns = totalRuns;
    newPlayer->battingAverage = battingAverage;
    newPlayer->strikeRate = strikeRate;
    newPlayer->wicketsTaken = wicketsTaken;
    newPlayer->economyRate = economyRate;
    newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
    newPlayer->next = NULL;
    return newPlayer;
}

// Appends a player node to the end of the linked list of players for a team
void appendPlayerToTeam(PlayerNode** head, PlayerNode* newPlayer) 
{
    if (*head == NULL) 
    {
        *head = newPlayer;
    } 
    else
    {
        PlayerNode* current = *head;
        while (current->next != NULL) 
        {
            current = current->next;
        }
        current->next = newPlayer;
    }
}

// Finds a team index in the array using its numeric team ID (via binary search)
int findTeamIndexById(int teamId) 
{
    int low = 0;
    int high = totalTeams - 1;
    while (low <= high) 
    {
        int middle = (low + high) / 2;
        if (allTeams[middle].teamId == teamId) 
        {
            return middle;
        } 
        else if (allTeams[middle].teamId < teamId) 
        {
            low = middle + 1;
        } 
        else 
        {
            high = middle - 1;
        }
    }
    return -1;
}

// Finds a team index by matching its team name (case insensitive)
int findTeamIndexByName(const char* teamName) 
{
    for (int teamIndex = 0; teamIndex < totalTeams; ++teamIndex) 
    {
        if (compareStringsCaseInsensitive(allTeams[teamIndex].teamName, teamName) == 0) 
        {
            return teamIndex;
        }
    }
    return -1;
}

// Recomputes and updates a team’s total players and average batting strike rate
void updateTeamStatistics(Team* team) 
{
    int totalRelevantPlayers = 0;
    float totalStrikeRateSum = 0.0f;
    int totalPlayersInTeam = 0;

    PlayerNode* currentPlayer = team->playerList;
    while (currentPlayer != NULL) 
    {
        totalPlayersInTeam++;
        if (strcmp(currentPlayer->role, "Batsman") == 0 || strcmp(currentPlayer->role, "All-rounder") == 0) 
        {
            totalStrikeRateSum += currentPlayer->strikeRate;
            totalRelevantPlayers++;
        }
        currentPlayer = currentPlayer->next;
    }
    team->totalPlayers = totalPlayersInTeam;
    team->averageBattingStrikeRate = (totalRelevantPlayers > 0) ? totalStrikeRateSum / totalRelevantPlayers : 0.0f;
}

// Prints one player's complete details in a formatted table row
void printPlayerDetails(const PlayerNode* player, int includeTeamName) 
{
    if (includeTeamName) 
    {
        printf("%-5d %-20s %-15s %-12s %-7d %-7.1f %-7.1f %-7d %-7.1f %8.2f\n",
               player->playerId, player->playerName, player->teamName, player->role,
               player->totalRuns, player->battingAverage, player->strikeRate,
               player->wicketsTaken, player->economyRate, player->performanceIndex);
    } else 
    {
        printf("%-5d %-20s %-12s %-7d %-7.1f %-7.1f %-7d %-7.1f %8.2f\n",
               player->playerId, player->playerName, player->role, player->totalRuns,
               player->battingAverage, player->strikeRate, player->wicketsTaken,
               player->economyRate, player->performanceIndex);
    }
}

// Initializes all teams and player lists using data from Players_data.h
void initializeTeamsAndPlayers() 
{
    totalTeams = teamCount;
    allTeams = (Team*)malloc(sizeof(Team) * totalTeams);
    if (!allTeams) 
    {
        fprintf(stderr, "Memory allocation failed for teams\n");
        exit(EXIT_FAILURE);
    }

    for (int teamIndex = 0; teamIndex < totalTeams; ++teamIndex) 
    {
        allTeams[teamIndex].teamId = teamIndex + 1;
        strncpy(allTeams[teamIndex].teamName, teams[teamIndex], MAX_TEAM_NAME_LENGTH - 1);
        allTeams[teamIndex].teamName[MAX_TEAM_NAME_LENGTH - 1] = '\0';
        allTeams[teamIndex].playerList = NULL;
        allTeams[teamIndex].totalPlayers = 0;
        allTeams[teamIndex].averageBattingStrikeRate = 0.0f;
    }

    for (int playerIndex = 0; playerIndex < playerCount; ++playerIndex) 
    {
        const Player* playerData = &players[playerIndex];
        int teamIndex = findTeamIndexByName(playerData->team);
        if (teamIndex == -1) 
        {
            continue;
        }

        PlayerNode* newPlayer = createPlayerNode(
            playerData->id, playerData->name, playerData->team,
            playerData->role, playerData->totalRuns, playerData->battingAverage,
            playerData->strikeRate, playerData->wickets, playerData->economyRate
        );

        appendPlayerToTeam(&allTeams[teamIndex].playerList, newPlayer);
    }

    for (int teamIndex = 0; teamIndex < totalTeams; ++teamIndex) 
    {
        updateTeamStatistics(&allTeams[teamIndex]);
    }
}

// Merge sort helpers
void merge(PlayerNode* playerArray[], int leftIndex, int middleIndex, int rightIndex) 
{
    int leftSize = middleIndex - leftIndex + 1;
    int rightSize = rightIndex - middleIndex;

    PlayerNode* leftArray[leftSize];
    PlayerNode* rightArray[rightSize];

    for (int leftPointer = 0; leftPointer < leftSize; ++leftPointer)
    {
        leftArray[leftPointer] = playerArray[leftIndex + leftPointer];
    }
    for (int rightPointer = 0; rightPointer < rightSize; ++rightPointer)
    {
        rightArray[rightPointer] = playerArray[middleIndex + 1 + rightPointer];
    }

    int leftPointer = 0, rightPointer = 0, mergedPointer = leftIndex;
    while (leftPointer < leftSize && rightPointer < rightSize) 
    {
        if (leftArray[leftPointer]->performanceIndex >= rightArray[rightPointer]->performanceIndex)
        {
            playerArray[mergedPointer++] = leftArray[leftPointer++];
        }
        else
        {
            playerArray[mergedPointer++] = rightArray[rightPointer++];
        }
    }

    while (leftPointer < leftSize) 
    {
        playerArray[mergedPointer++] = leftArray[leftPointer++];
    }
    while (rightPointer < rightSize) 
    {
        playerArray[mergedPointer++] = rightArray[rightPointer++];
    }
}

// Recursively sorts player nodes by performance index using Merge Sort.
void mergeSort(PlayerNode* playerArray[], int leftIndex, int rightIndex) 
{
    if (leftIndex < rightIndex) 
    {
        int middleIndex = (leftIndex + rightIndex) / 2;
        mergeSort(playerArray, leftIndex, middleIndex);
        mergeSort(playerArray, middleIndex + 1, rightIndex);
        merge(playerArray, leftIndex, middleIndex, rightIndex);
    }
}

// Quickselect helpers
int partitionForQuickSelect(PlayerNode* playerArray[], int lowIndex, int highIndex) 
{
    float pivotValue = playerArray[highIndex]->performanceIndex;
    int smallerBoundary = lowIndex - 1;

    for (int scanningIndex = lowIndex; scanningIndex < highIndex; scanningIndex++) 
    {
        if (playerArray[scanningIndex]->performanceIndex >= pivotValue) 
        {
            smallerBoundary++;
            PlayerNode* temp = playerArray[smallerBoundary];
            playerArray[smallerBoundary] = playerArray[scanningIndex];
            playerArray[scanningIndex] = temp;
        }
    }

    PlayerNode* temp = playerArray[smallerBoundary + 1];
    playerArray[smallerBoundary + 1] = playerArray[highIndex];
    playerArray[highIndex] = temp;
    return smallerBoundary + 1;
}

void quickSelect(PlayerNode* playerArray[], int lowIndex, int highIndex, int k) 
{
    if (lowIndex < highIndex) 
    {
        int partitionIndex = partitionForQuickSelect(playerArray, lowIndex, highIndex);
        int leftPartitionSize = partitionIndex - lowIndex + 1;
        if (leftPartitionSize == k) 
        {
            return;
        }
        else if (leftPartitionSize > k)
        {
            quickSelect(playerArray, lowIndex, partitionIndex - 1, k);
        }
        else
        {
            quickSelect(playerArray, partitionIndex + 1, highIndex, k - leftPartitionSize);
        }
    }
}

// Adds a new player to a specified team via user input
void addPlayerToTeam() 
{
    int teamId;
    printf("Enter Team ID to add player: ");
    if (scanf("%d", &teamId) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    int teamIndex = findTeamIndexById(teamId);
    if (teamIndex == -1) 
    {
        printf("Invalid team ID!\n");
        return;
    }

    int playerId, totalRuns, wicketsTaken;
    float battingAverage, strikeRate, economyRate;
    char playerName[MAX_NAME_LENGTH];
    int roleChoice;
    char role[MAX_ROLE_LENGTH];

    printf("Enter Player Details:\n");
    printf("Player ID: ");
    if (scanf("%d", &playerId) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    getchar();
    printf("Name: ");
    if (!fgets(playerName, sizeof(playerName), stdin)) 
    {
        return;
    }
    playerName[strcspn(playerName, "\n")] = 0;

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    if (roleChoice == 1) 
    {
        strcpy(role, "Batsman");
    }
    else if (roleChoice == 2) 
    {
        strcpy(role, "Bowler");
    }
    else 
    {
        strcpy(role, "All-rounder");
    }

    printf("Total Runs: ");
    if (scanf("%d", &totalRuns) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    printf("Batting Average: ");
    if (scanf("%f", &battingAverage) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    printf("Strike Rate: ");
    if (scanf("%f", &strikeRate) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    printf("Wickets: ");
    if (scanf("%d", &wicketsTaken) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    printf("Economy Rate: ");
    if (scanf("%f", &economyRate) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }

    PlayerNode* newPlayer = createPlayerNode(playerId, playerName, allTeams[teamIndex].teamName,
                                             role, totalRuns, battingAverage, strikeRate,
                                             wicketsTaken, economyRate);

    appendPlayerToTeam(&allTeams[teamIndex].playerList, newPlayer);
    updateTeamStatistics(&allTeams[teamIndex]);
    printf("Player added successfully to Team %s!\n", allTeams[teamIndex].teamName);
}


// Displays all players belonging to a specific team
void displayPlayersOfTeam() 
{
    int teamId;
    printf("Enter Team ID: ");
    if (scanf("%d", &teamId) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    int teamIndex = findTeamIndexById(teamId);
    if (teamIndex == -1) 
    {
        printf("Invalid team!\n");
        return;
    }

    Team* selectedTeam = &allTeams[teamIndex];
    printf("\nPlayers of Team %s:\n", selectedTeam->teamName);
    printf("====================================================================================\n");
    printf("ID    Name                 Team            Role         Runs   Avg     SR      Wkts    ER     PerfIdx\n");
    printf("====================================================================================\n");
    PlayerNode* currentPlayer = selectedTeam->playerList;
    while (currentPlayer) 
    {
        printPlayerDetails(currentPlayer, 1);
        currentPlayer = currentPlayer->next;
    }
    printf("====================================================================================\n");
    printf("Total Players: %d\nAverage Batting Strike Rate: %.2f\n",
           selectedTeam->totalPlayers, selectedTeam->averageBattingStrikeRate);
}

// Displays all teams sorted by their average batting strike rate
void displayTeamsByAverageStrikeRate() 
{
    Team sortedTeams[totalTeams];
    for (int copyIndex = 0; copyIndex < totalTeams; ++copyIndex) 
    {
        sortedTeams[copyIndex] = allTeams[copyIndex];
    }

    for (int outerIndex = 0; outerIndex < totalTeams - 1; ++outerIndex) 
    {
        for (int innerIndex = 0; innerIndex < totalTeams - outerIndex - 1; ++innerIndex) 
        {
            if (sortedTeams[innerIndex].averageBattingStrikeRate < sortedTeams[innerIndex + 1].averageBattingStrikeRate) 
            {
                Team temp = sortedTeams[innerIndex];
                sortedTeams[innerIndex] = sortedTeams[innerIndex + 1];
                sortedTeams[innerIndex + 1] = temp;
            }
        }
    }

    printf("\nTeams Sorted by Average Batting Strike Rate:\n");
    printf("=========================================================\n");
    printf("ID   Team Name             Avg SR     Total Players\n");
    printf("=========================================================\n");
    for (int printIndex = 0; printIndex < totalTeams; ++printIndex) {
        printf("%-4d %-22s %-10.2f %-10d\n",
               sortedTeams[printIndex].teamId, sortedTeams[printIndex].teamName,
               sortedTeams[printIndex].averageBattingStrikeRate, sortedTeams[printIndex].totalPlayers);
    }
    printf("=========================================================\n");
}

// Displays the top K players of a particular role from a specific team
void displayTopKPlayersOfTeamByRole() 
{
    int teamId;
    int roleChoice;
    int topK;

    printf("Enter Team ID: ");
    if (scanf("%d", &teamId) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    int teamIndex = findTeamIndexById(teamId);
    if (teamIndex == -1) 
    {
        printf("Invalid team!\n");
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    char role[MAX_ROLE_LENGTH];
    if (roleChoice == 1) 
    {
        strcpy(role, "Batsman");
    }
    else if (roleChoice == 2) 
    {
        strcpy(role, "Bowler");
    }
    else 
    {
        strcpy(role, "All-rounder");
    }

    printf("Enter number of players: ");
    if (scanf("%d", &topK) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }

    PlayerNode* playerArray[200];
    int collectedCount = 0;
    PlayerNode* currentPlayer = allTeams[teamIndex].playerList;
    while (currentPlayer) 
    {
        if (strcmp(currentPlayer->role, role) == 0) 
        {
            playerArray[collectedCount++] = currentPlayer;
        }
        currentPlayer = currentPlayer->next;
    }

    if (collectedCount == 0) 
    {
        printf("No players of role %s in team %s.\n", role, allTeams[teamIndex].teamName);
        return;
    }

    if (topK > collectedCount) 
    {
        topK = collectedCount;
    }
    quickSelect(playerArray, 0, collectedCount - 1, topK);

    printf("\nTop %d %s(s) of Team %s:\n", topK, role, allTeams[teamIndex].teamName);
    printf("====================================================================================\n");
    printf("ID    Name                 Role         Runs   Avg     SR      Wkts    ER     PerfIdx\n");
    printf("====================================================================================\n");
    for (int printIndex = 0; printIndex < topK; ++printIndex) printPlayerDetails(playerArray[printIndex], 0);
    printf("====================================================================================\n");
}

// Displays all players across all teams having a specific role, sorted by performance.
void displayAllPlayersByRole() 
{
    int roleChoice;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1) 
    { 
        while (getchar() != '\n'); 
        printf("Invalid input\n"); 
        return; 
    }
    char role[MAX_ROLE_LENGTH];
    if (roleChoice == 1) 
    {
        strcpy(role, "Batsman");
    }
    else if (roleChoice == 2) 
    {
        strcpy(role, "Bowler");
    }
    else 
    {
        strcpy(role, "All-rounder");
    }

    PlayerNode* playerArray[1000];
    int collectedCount = 0;
    for (int teamIndex = 0; teamIndex < totalTeams; ++teamIndex) {
        PlayerNode* currentPlayer = allTeams[teamIndex].playerList;
        while (currentPlayer) {
            if (strcmp(currentPlayer->role, role) == 0) playerArray[collectedCount++] = currentPlayer;
            currentPlayer = currentPlayer->next;
        }
    }

    if (collectedCount == 0) {
        printf("No players of role %s found.\n", role);
        return;
    }

    mergeSort(playerArray, 0, collectedCount - 1);

    printf("\nAll %s(s) Across All Teams:\n", role);
    printf("====================================================================================\n");
    printf("ID    Name                 Team            Role         Runs   Avg     SR      Wkts    ER     PerfIdx\n");
    printf("====================================================================================\n");
    for (int printIndex = 0; printIndex < collectedCount; ++printIndex) printPlayerDetails(playerArray[printIndex], 1);
    printf("====================================================================================\n");
}

// Frees all dynamically allocated memory for teams and players before exit
void freeAllAllocatedMemory() 
{
    if (!allTeams)
    {
        return;
    }
    for (int teamIndex = 0; teamIndex < totalTeams; ++teamIndex) 
    {
        PlayerNode* currentPlayer = allTeams[teamIndex].playerList;
        while (currentPlayer) 
        {
            PlayerNode* nextPlayer = currentPlayer->next;
            free(currentPlayer);
            currentPlayer = nextPlayer;
        }
    }
    free(allTeams);
}

int main() 
{
    initializeTeamsAndPlayers();

    int userChoice;
    while (1) 
    {
        printf("\n================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of Specific Role Across All Teams\n");
        printf("6. Exit\n");
        printf("================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &userChoice) != 1) 
        {
            while (getchar() != '\n'); // flush
            printf("Invalid input. Please enter a number between 1 and 6.\n");
            continue;
        }

        switch (userChoice) 
        {
            case 1: 
                addPlayerToTeam(); 
                break;
            case 2: 
                displayPlayersOfTeam(); 
                break;
            case 3: 
                displayTeamsByAverageStrikeRate(); 
                break;
            case 4: 
                displayTopKPlayersOfTeamByRole(); 
                break;
            case 5: 
                displayAllPlayersByRole(); 
                break;
            case 6:
                freeAllAllocatedMemory();
                printf("Exiting... Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
