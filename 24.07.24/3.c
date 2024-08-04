#include <stdio.h>
#include <stdint.h>

int main()
{
    unsigned int number;
    char byte1, byte2, byte3, byte4;

    printf("Enter an unsigned integer number: ");
    scanf("%d", &number);

    byte1 = number & 0xFF;
    byte2 = (number >> 8) & 0xFF;
    byte3 = (number >> 16) & 0xFF;
    byte4 = (number >> 24) & 0xFF;

    printf("Lowest byte: %02X\n", byte1);
    printf("Second lowest byte: %02X\n", byte2);
    printf("Third lowest byte: %02X\n", byte3);
    printf("Highest byte: %02X\n", byte4);

    return 0;
}