#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Enter two variables");
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    printf("Original values: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("Swapped values: a = %d, b = %d\n", a, b);

    return 0;
}