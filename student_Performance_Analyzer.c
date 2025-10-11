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
    studentPtr->averageMarks=studentPtr->totalMarks/(double)SUBJECTS;
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


int main()
{
    int studentNumber=0;
    scanf("%d",&studentNumber);

    Student students[100];

    for(int studentIndex = 0; studentIndex < studentNumber; studentIndex++)
    {
        scanf("%d %s", &students[studentIndex].rollNo,students[studentIndex].name);
        
        // Using a loop makes it easy to handle any number of subjects
        // without changing the code for each subject individually
        for(int subjectIndex = 0; subjectIndex < SUBJECTS; subjectIndex++)
        {
            scanf("%d",&students[studentIndex].marks[subjectIndex]);
        }

        calculateTotalMarks(&students[studentIndex]);
        calculateAverageMarks(&students[studentIndex]);
        generateGrade(&students[studentIndex]);

    }
    for(int studentIndex = 0; studentIndex < studentNumber; studentIndex++)
    {
        printf("Roll: %d\n", students[studentIndex].rollNo);
        printf("Name: %s\n", students[studentIndex].name);
        printf("Total: %d\n", students[studentIndex].totalMarks);
        printf("Average: %.2lf\n", students[studentIndex].averageMarks);
        printf("Grade: %c\n", gradeToChar(students[studentIndex].grade));

        if (students[studentIndex].averageMarks < 35) 
        {
            printf("\n");
            continue;
        }

        printPerformance(students[studentIndex].grade);        
    }
    printf("List of Roll Number (via recursion): ");
    printRollNumbers(students, studentNumber, 0);
    printf("\n");
    return 0;
}
