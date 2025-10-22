#include<stdio.h>

# define SUBJECTS 3
//stores grade points
enum Grade { F, D, C, B, A };

//stores student details
typedef struct student
{
    int rollNo;
    char name[100];
    int marks[SUBJECTS];
    int totalMarks;
    double averageMarks;
    enum Grade grade;
}Student;

//computes tolal marks of 3 subjects
void calculateTotalMarks(Student *studentPtr)
{
    studentPtr->totalMarks=0;
    for(int subjectIndex = 0; subjectIndex < SUBJECTS; subjectIndex++)
    {
       studentPtr->totalMarks += studentPtr->marks[subjectIndex];
    }
}

//computes average of 3 subjects
void calculateAverageMarks(Student *studentPtr)
{
    studentPtr->averageMarks=(double)studentPtr->totalMarks/SUBJECTS;
}

// computes grade based on average
void generateGrade(Student *studentPtr)
{
    if(studentPtr->averageMarks >= 85)
    {
        studentPtr->grade = A;
    }
    else if(studentPtr->averageMarks >= 70)
    {
        studentPtr->grade = B;
    }
    else if(studentPtr->averageMarks >= 50)
    {
        studentPtr->grade = C;
    }
    else if(studentPtr->averageMarks >= 35)
    {
        studentPtr->grade = D;
    }
    else 
    {
        studentPtr->grade = F;
    }
}

//assigns grade using enums
char gradeToChar(enum Grade grade) {
    switch (grade) {
        case A: return 'A';
        case B: return 'B';
        case C: return 'C';
        case D: return 'D';
        case F: return 'F';
    }
    return 'F'; // Default
}

//print performance based on grade
void printPerformance(enum Grade grade)
{
    printf("Performance: ");
    switch(grade)
    {
        case A: printf("*****"); 
                break;
        case B: printf("****"); 
                break;
        case C: printf("***"); 
                break;
        case D: printf("**");
                break;
        default: break;  
    }

    printf("\n");
}

//print roll numbers using recursion
void printRollNumbers(Student students[], int studentNumber, int index)
{
    if(index == studentNumber)
        return;
    printf("%d ", students[index].rollNo);
    printRollNumbers(students, studentNumber, index+1);
}

//Function to take student input
void inputStudents(Student students[], int studentNumber) {
    for (int i = 0; i < studentNumber; i++) {
        scanf("%d %s", &students[i].rollNo, students[i].name);
        // Using a loop makes it easy to handle any number of subjects
        // without changing the code for each subject individually
        for (int j = 0; j < SUBJECTS; j++) {
            scanf("%d", &students[i].marks[j]);
        }
        calculateTotalMarks(&students[i]);
        calculateAverageMarks(&students[i]);
        generateGrade(&students[i]);
    }
}

//Function to display student output
void displayStudents(Student students[], int studentNumber) {
    for (int i = 0; i < studentNumber; i++) {
        printf("Roll: %d\n", students[i].rollNo);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", students[i].totalMarks);
        printf("Average: %.2lf\n", students[i].averageMarks);
        printf("Grade: %c\n", gradeToChar(students[i].grade));

        if (students[i].averageMarks < 35) {
            printf("\n");
            continue;
        }
        printPerformance(students[i].grade);
    }
    printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(students, studentNumber, 0);
    printf("\n");
}
int main()
{
    int studentNumber=0;
    scanf("%d",&studentNumber);

    Student students[100];

    inputStudents(students, studentNumber);
    displayStudents(students, studentNumber);
    return 0;
}
