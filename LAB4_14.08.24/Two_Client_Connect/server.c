#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVPORT 3000
#define MAX_CLIENTS 2
#define MAXBUFLEN 100

int main()
{

    // Create TCP socket
    int serv_sock, cli_sock[MAX_CLIENTS];
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in sa, ca[MAX_CLIENTS];
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(SERVPORT);

    // Bind the server address to its port
    if ((bind(serv_sock, (struct sockaddr *)&sa, sizeof sa)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen for client requests
    if (listen(serv_sock, MAX_CLIENTS) == -1)
    {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening for %d clients...\n", MAX_CLIENTS);

    // Accept connections from two clients
    char buf[MAXBUFLEN];
    socklen_t length[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        length[i] = sizeof(ca[i]);
        if ((cli_sock[i] = accept(serv_sock, (struct sockaddr *)&ca[i], &length[i])) == -1)
        {
            perror("Accept failed");
            exit(1);
        }
        printf("Client %d connected\n", i + 1);

        // Send a message to the client
        memset(buf, '\0', MAXBUFLEN);
        sprintf(buf, "Hello Client %d!", i + 1);
        send(cli_sock[i], buf, MAXBUFLEN, 0);

        // Receive a message from the client
        memset(buf, '\0', MAXBUFLEN);
        recv(cli_sock[i], buf, MAXBUFLEN, 0);
        printf("Received from Client %d: %s\n", i + 1, buf);
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        close(cli_sock[i]);
    }
    close(serv_sock);

    return 0;
}