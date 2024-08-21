#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>

#define MAXCLIENTS 1
#define MAXBUFLEN 100
#define SERVPORT 3550

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
            // if (strlen(buf) + strlen(dir->d_name) + 2 < MAXBUFLEN)
            {
                strcat(buf, dir->d_name);
                strcat(buf, "\n");
            }
            // else
            // {
            //     printf("Buffer limit reached!\n");
            //     break;
            // }
        }

        closedir(d);
    }

    return buf;
}

int main()
{
    // Create TCP socket
    int serv_sock, cli_sock;
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

    // Accept request from client
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
    buf[recv_len] = '\0';
    printf("Received command: %s\n", buf);

    // Fill buffer with directory list
    char *response;
    if (strcmp(buf, "ls\n") == 0)
    {
        response = get_dir_list();
        if (!response)
        {
            /* Dynamically allocate memory as we free respose
            pointer later */
            response = strdup("Failed to get directory listing");
        }
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
    close(serv_sock);
    free(response);
    return 0;
}