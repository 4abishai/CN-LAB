#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXBUFLEN 100
#define SERVPORT 3000
#define DOMAINS 5
#define MAXLINELEN 1024

void handle_client(int cli_sock)
{
    char buf[MAXBUFLEN];
    memset(buf, '\0', MAXBUFLEN);

    // Receive message into buffer
    recv(cli_sock, buf, MAXBUFLEN, 0);
    buf[strcspn(buf, "\n")] = 0; // Remove newline
    int query_roll = atoi(buf);

    // Open student file
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr)
    {
        perror("Error opening file");
        send(cli_sock, "Error: Unable to access student data", MAXBUFLEN, 0);
        return;
    }

    // Search for the student detail
    int flag = 0;
    char line[MAXLINELEN];
    while (fgets(line, MAXLINELEN, fptr))
    {
        int roll;
        sscanf(line, "%d", &roll);
        if (roll == query_roll)
        {
            flag = 1;
            break;
        }
    }

    // Send the buffer to the client
    if (flag)
        send(cli_sock, line, strlen(line), 0);
    else
        send(cli_sock, "Student not found", MAXBUFLEN, 0);

    fclose(fptr);
}

int main()
{
    // Create TCP socket
    int serv_sock, cli_sock;
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(1);
    }

    // Configure address
    struct sockaddr_in sa, ca; // server address, client address
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(SERVPORT);

    // Bind the socket with the server address
    if ((bind(serv_sock, (struct sockaddr *)&sa, sizeof sa)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    // Atmost 5 requests can queue up for connection
    if (listen(serv_sock, 5) == -1)
    {
        perror("Listen failed");
        exit(1);
    }
    printf("Server is listening...\n");

    while (1)
    {
        // Accept an incoming connection from a client
        socklen_t length = sizeof ca;
        if ((cli_sock = accept(serv_sock, (struct sockaddr *)&ca, &length)) == -1)
        {
            perror("Accept failed");
            exit(1);
        }

        handle_client(cli_sock);
        close(cli_sock);
    }

    close(serv_sock);
    return 0;
}
