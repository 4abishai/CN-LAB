#include <stdio.h>

int main()
{
    unsigned int x = -23;
    char *c = (char *)&x;

    printf("Byte representation of %u:\n", x);

    for (int i = 0; i < sizeof(x); i++)
    {
        printf("Byte %d: %02x\n", i, (unsigned char)c[i]);
    }

    if (*c)
    {
        printf("System is Little-endian\n");
    }
    else
    {
        printf("System is Big-endian\n");
    }

    return 0;
}
