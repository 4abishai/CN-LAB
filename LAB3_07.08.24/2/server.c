#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYPORT 4952   // The port on which the server will listen
#define MAXBUFLEN 200 // Maximum buffer length

int main()
{
    int sockfd;                             // Socket file descriptor
    struct sockaddr_in my_addr, their_addr; // My address and client address
    socklen_t addr_len;                     // Size of the address struct
    int numbytes;                           // Number of bytes sent or received
    char buf[MAXBUFLEN];                    // Buffer to store data

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Configure my address
    my_addr.sin_family = AF_INET;                            // IPv4
    my_addr.sin_port = htons(MYPORT);                        // My port
    my_addr.sin_addr.s_addr = INADDR_ANY;                    // Accept any incoming address
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero); // Zero the rest of the struct

    // Bind the socket to my address
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1)
    {
        perror("bind");
        exit(1);
    }

    addr_len = sizeof their_addr;

    printf("Server is running and waiting for data...\n");

    // Receive data from the client
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }
    buf[numbytes] = '\0'; // Null-terminate the received data

    printf("Got packet from %s\n", inet_ntoa(their_addr.sin_addr));
    printf("Packet contains \"%s\"\n", buf);

    // Extract the two integers from the received data
    int num1, num2, result;
    sscanf(buf, "%d %d", &num1, &num2);
    result = num1 + num2;

    // Store the result in the buffer
    sprintf(buf, "%d", result);

    // Send the result back to the client
    if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
                           (struct sockaddr *)&their_addr, sizeof their_addr)) == -1)
    {
        perror("sendto");
        exit(1);
    }

    printf("Sent result %s to %s\n", buf, inet_ntoa(their_addr.sin_addr));

    // Close the socket
    close(sockfd);
    return 0;
}
