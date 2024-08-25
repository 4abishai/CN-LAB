#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>
#include <errno.h>

#define MAXCLIENTS 1
#define MAXBUFLEN 5000
#define SERVPORT 3002

void print(const char *buf)
{
    for (int i = 0; i < strlen(buf); i++)
    {
        printf("%c", buf[i]);
    }
}

char *get_dir_list()
{
    char *buf = (char *)malloc(MAXBUFLEN * sizeof(char));
    if (!buf)
        return NULL;
    buf[0] = '\0'; // Init with null string

    DIR *d = opendir(".");
    if (d)
    {
        struct dirent *dir;
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

char *get_content(char *file)
{
    FILE *fptr = fopen(file, "r");
    if (fptr == NULL)
    {
        return strdup("Unable to open file");
    }
    char *content = (char *)malloc(MAXBUFLEN * sizeof(char));
    if (!content)
    {
        fclose(fptr);
        return strdup("Memory allocation failed");
    }
    size_t bytes_read = fread(content, 1, MAXBUFLEN - 1, fptr);
    fclose(fptr);
    if (bytes_read == 0)
    {
        free(content);
        return strdup("File is empty or read failed");
    }
    content[bytes_read] = '\0';
    return content;
}

int main()
{
    // Create TCP socket
    int serv_sock;
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket faulted");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in sa, ca;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERVPORT);
    sa.sin_addr.s_addr = INADDR_ANY;

    // Bind the server address to its port
    if ((bind(serv_sock, (struct sockaddr *)&sa, sizeof sa)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen for clients; queue upto MAXCLIENTS
    if (listen(serv_sock, MAXCLIENTS) == -1)
    {
        perror("Listen faulted");
        exit(1);
    }
    printf("Server is listening...\n");

    while (1)
    {
        // Accept request from client
        int cli_sock;
        socklen_t length = sizeof ca;
        if ((cli_sock = accept(serv_sock, (struct sockaddr *)&ca, &length)) == -1)
        {
            perror("Accept faulted");
            exit(1);
        }

        // Receive cmd buffer
        char buf[MAXBUFLEN] = {0};
        int recv_len = 0;
        if ((recv_len = recv(cli_sock, buf, MAXBUFLEN, 0)) == -1)
        {
            perror("Receive faulted");
            exit(1);
        }
        // Remove newline character
        buf[strcspn(buf, "\n")] = 0;

        // Fill buffer with directory list
        char *response;
        if (strncmp(buf, "ls", 2) == 0)
        {
            response = get_dir_list();
            if (!response)
            {
                response = strdup("Failed to get directory listing");
            }
        }
        else if (strncmp(buf, "cat ", 4) == 0)
        {
            response = get_content(buf + 4); // Skip "cat " prefix
        }
        else
        {
            response = strdup("Command not available");
        }

        // Send the buffer to the client
        if (send(cli_sock, response, strlen(response), 0) == -1)
        {
            perror("Send faulted");
            exit(1);
        }

        close(cli_sock);
        free(response);
    }
    close(serv_sock);
    return 0;
}