#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client1_addr, client2_addr;
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to server address
    bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server listening on port %d...\n", PORT);

    // Receive from client 1
    int recv_len = recvfrom(sockfd, buffer1, BUFFER_SIZE, 0,
                            (struct sockaddr *)&client1_addr, &addr_len);
    buffer1[recv_len] = '\0';
    printf("Received from client 1: %s\n", buffer1);

    // Receive from client 2
    recv_len = recvfrom(sockfd, buffer2, BUFFER_SIZE, 0,
                        (struct sockaddr *)&client2_addr, &addr_len);
    buffer2[recv_len] = '\0';
    printf("Received from client 2: %s\n", buffer2);

    // Forward client 1's message to client 2
    sendto(sockfd, buffer1, strlen(buffer1), 0,
           (const struct sockaddr *)&client2_addr, sizeof(client2_addr));
    printf("Forwarded to client 2: %s\n", buffer1);

    // Forward client 2's message to client 1
    sendto(sockfd, buffer2, strlen(buffer2), 0,
           (const struct sockaddr *)&client1_addr, sizeof(client1_addr));
    printf("Forwarded to client 1: %s\n", buffer2);

    close(sockfd);
    return 0;
}