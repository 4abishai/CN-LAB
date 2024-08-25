#include <stdio.h>

void printBytes(unsigned int num)
{
    unsigned char *bytePtr = (unsigned char *)&num;
    for (int i = 0; i < sizeof(num); i++)
    {
        printf("Byte %d: 0x%02X\n", i, bytePtr[i]);
    }
}

unsigned int convertEndianness(unsigned int num)
{
    unsigned int converted = ((num >> 24) & 0x000000FF) |
                             ((num >> 8) & 0x0000FF00) |
                             ((num << 8) & 0x00FF0000) |
                             ((num << 24) & 0xFF000000);
    return converted;
}

void checkEndianness()
{
    unsigned int x = 1;

    // By casting the pointer to unsigned char*, memory address can be
    // treated as pointing to a series of bytes rather than a single integer.
    // [0] points to the least significant byte
    char LSB = ((char *)&x)[0];

    // Use ternary operator to check and print endianness
    printf("The system is %s Endian\n", LSB ? "Little" : "Big");
}

int main()
{
    checkEndianness();

    unsigned int num;

    printf("Enter a number: ");
    scanf("%u", &num);

    printf("Original number in bytes:\n");
    printBytes(num);

    unsigned int convertedNum = convertEndianness(num);

    printf("Converted number in bytes:\n");
    printBytes(convertedNum);

    return 0;
}
