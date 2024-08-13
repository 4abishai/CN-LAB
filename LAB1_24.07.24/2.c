#include <stdio.h>
#include <stdlib.h>

struct dob
{
    int day;
    int month;
    int year;
};

struct student_info
{
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
};

void assign_call_by_val(struct student_info stud)
{
    printf("Call by Value:\n");
    printf("Roll No: %d\n", stud.roll_no);
    printf("Name: %s\n", stud.name);
    printf("CGPA: %f\n", stud.CGPA);
    printf("Date of Birth: %d/%d/%d\n", stud.age.day, stud.age.month, stud.age.year);
}

void assign_call_by_ref(struct student_info *stud)
{
    printf("Call by Reference:\n");
    printf("Roll No: %d\n", stud->roll_no);
    printf("Name: %s\n", stud->name);
    printf("CGPA: %f\n", stud->CGPA);
    printf("Date of Birth: %d/%d/%d\n", stud->age.day, stud->age.month, stud->age.year);
}

int main()
{
    struct student_info stud;

    printf("Enter:\n");
    printf("Roll No: ");
    scanf("%d", &stud.roll_no);
    printf("Name: ");
    scanf("%s", stud.name);
    printf("CGPA: ");
    scanf("%f", &stud.CGPA);
    printf("DOB (day month year): ");
    scanf("%d %d %d", &stud.age.day, &stud.age.month, &stud.age.year);

    assign_call_by_val(stud);
    assign_call_by_ref(&stud);

    return 0;
}
