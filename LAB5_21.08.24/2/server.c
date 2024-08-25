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

typedef struct
{
    char domain[50];
    char ip[16];
} DomainMap;

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

    // Accept an incoming connection from a client
    socklen_t length = sizeof ca;
    if ((cli_sock = accept(serv_sock, (struct sockaddr *)&ca, &length)) == -1)
    {
        perror("Accept failed");
        exit(1);
    }

    // Receive message into buffer
    char buf[100];
    memset(buf, '\0', 100);
    recv(cli_sock, buf, 100, 0);
    buf[strcspn(buf, "\n")] = 0; // Remove newline

    printf("Received domain: %s\n", buf);

    // Domain table for domains and IP address
    DomainMap domain_map[DOMAINS] = {
        {"example.com", "192.168.1.1"},
        {"test.com", "192.168.1.2"},
        {"hello.com", "192.168.1.3"},
        {"world.com", "192.168.1.4"},
        {"pseudo.com", "192.168.1.5"}};

    // Search for the domain name
    for (int i = 0; i < DOMAINS; i++)
        if (strcmp(buf, domain_map[i].domain) == 0)
        {
            // Put ip address onto buffer
            memset(buf, '\0', MAXBUFLEN);
            sprintf(buf, "%s", domain_map[i].ip);
        }

    // Send the buffer to the client
    send(cli_sock, buf, strlen(buf), 0);

    close(serv_sock);
    close(cli_sock);
    return 0;
}
