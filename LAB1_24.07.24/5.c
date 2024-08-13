#include <stdio.h>

struct pkt
{
    char ch1;
    char ch2[2];
    char ch3;
};

int main()
{
    unsigned int num;
    struct pkt p;

    printf("Enter a number (0-65535): ");
    scanf("%u", &num);

    // Ensure the number fits into 16 bits
    if (num > 65535)
    {
        printf("Number out of range!\n");
        return 1;
    }

    p.ch1 = num & 0xF;           // Lowest 4 bits
    p.ch2[0] = (num >> 4) & 0xF; // Next 4 bits
    p.ch2[1] = (num >> 8) & 0xF; // Next 4 bits
    p.ch3 = (num >> 12) & 0xF;   // Highest 4 bits

    printf("p.ch1: %X\n", p.ch1);
    printf("p.ch2[0]: %X\n", p.ch2[0]);
    printf("p.ch2[1]: %X\n", p.ch2[1]);
    printf("p.ch3: %X\n", p.ch3);

    // Aggregation
    int aggregated_num = p.ch1 | (p.ch2[0] << 4) | (p.ch2[1] << 8) | (p.ch3 << 12);
    printf("Aggregated number: %d\n", aggregated_num);

    return 0;
}
