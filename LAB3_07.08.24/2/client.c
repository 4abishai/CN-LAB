#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT 4952 // The port on which the server will listen
#define MAXBUFLEN 200   // Maximum buffer length
int main()
{
    int sockfd;                    // Socket file descriptor
    struct sockaddr_in their_addr; // Server's address information
    int numbytes;                  // Number of bytes sent or received
    char buf[MAXBUFLEN];           // Buffer to store data

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    // Configure server address
    their_addr.sin_family = AF_INET;                               // IPv4
    their_addr.sin_port = htons(SERVERPORT);                       // Server port
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");           // Server IP address
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero); // Zero the rest of the struct

    // Get user input (two integers) and store it in the buffer
    int num1, num2;
    printf("Enter two integers (or 0 0 to exit): ");
    scanf("%d %d", &num1, &num2);
    sprintf(buf, "%d %d", num1, num2);

    // Send the buffer to the server
    if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
                           (struct sockaddr *)&their_addr, sizeof their_addr)) == -1)
    {
        perror("sendto");
        exit(1);
    }
    printf("Sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));

    // Receive the result from the server
    socklen_t addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }
    buf[numbytes] = '\0'; // Null-terminate the received data

    printf("Received result from server: %s\n", buf);

    // Close the socket
    close(sockfd);
    return 0;
}
