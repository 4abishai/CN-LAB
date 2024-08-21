#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAXBUFLEN 100

char *get_dir_list()
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    char *buf = (char *)malloc(MAXBUFLEN * sizeof(char));
    if (!buf)
        return NULL;

    buf[0] = '\0'; // Init with null string

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {

            // Check if concatenation would exceed the buffer
            if (strlen(buf) + strlen(dir->d_name) + 2 < MAXBUFLEN)
            {
                strcat(buf, dir->d_name);
                strcat(buf, "\n");
            }
            else
            {
                printf("Buffer limit reached!\n");
                break;
            }
        }

        closedir(d);
    }

    return buf;
}

int main(void)
{
    char *buf = get_dir_list();

    printf("Concatenated buffer:\n%s", buf);

    free(buf);
    return 0;
}
