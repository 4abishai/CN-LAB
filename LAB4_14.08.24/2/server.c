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
    int serv_sock, cli_sock1, cli_sock2;
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in sa, ca1, ca2; // Server address, client address 1, client address 2
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(SERVPORT);

    // Bind the server address to its port
    if ((bind(serv_sock, (struct sockaddr *)&sa, sizeof sa)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen for client requests (queue upto MAX_CLIENTS)
    if (listen(serv_sock, MAX_CLIENTS) == -1)
    {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening for %d clients...\n", MAX_CLIENTS);

    // Accept connections from two clients

    // Connect client 1
    socklen_t length1 = sizeof(ca1);
    if ((cli_sock1 = accept(serv_sock, (struct sockaddr *)&ca1, &length1)) == -1)
    {
        perror("Accept failed");
        exit(1);
    }
    printf("Client 1 connected\n");

    // Connect client 2
    socklen_t length2 = sizeof(ca1);
    if ((cli_sock2 = accept(serv_sock, (struct sockaddr *)&ca2, &length2)) == -1)
    {
        perror("Accept failed");
        exit(1);
    }
    printf("Client 2 connected\n");

    // Receive a message from the client 1 and client 2
    char buf1[MAXBUFLEN], buf2[MAXBUFLEN];
    memset(buf1, '\0', MAXBUFLEN);
    memset(buf2, '\0', MAXBUFLEN);

    recv(cli_sock1, buf1, MAXBUFLEN, 0);
    printf("Received from Client 1: %s\n", buf1);

    recv(cli_sock2, buf2, MAXBUFLEN, 0);
    printf("Received from Client 2: %s\n", buf2);

    // Send messages to the opposite clients
    send(cli_sock1, buf2, MAXBUFLEN, 0);
    send(cli_sock2, buf1, MAXBUFLEN, 0);

    close(serv_sock);
    close(cli_sock1);
    close(cli_sock2);
    return 0;
}