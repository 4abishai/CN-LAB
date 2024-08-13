#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Enter a number to send: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // Send data to server
    sendto(sockfd, buffer, strlen(buffer), 0,
           (const struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive response from server
    int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                            NULL, NULL);

    buffer[recv_len] = '\0';
    printf("Received from other client: %s\n", buffer);

    close(sockfd);
    return 0;
}