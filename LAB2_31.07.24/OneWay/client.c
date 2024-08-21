#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVERPORT 4952 // the port users will be connecting to

int main()
{
    // STEP 1: Create UDP socket
    int sockfd; // socket field descriptor
    if ((sockfd = socket(
             AF_INET,    // Address family (IPV4)
             SOCK_DGRAM, // Datagram Socket
             0           // Default Protocol (UDP)
             )) == -1)
    {
        perror("socket");
        exit(1);
    }

    // STEP 2: Store server's address information
    struct sockaddr_in their_addr;
    their_addr.sin_family = AF_INET;                               // Address family (byte order)
    their_addr.sin_port = htons(SERVERPORT);                       // Server's Port converted to network byte order
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");           // Server's IP address
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero); // Zero the rest of the struct

    // STEP 3: Get user input
    char arg[30]; // Buffer storing msg
    printf("Enter a message\n");
    // gets(arg);
    fgets(arg, sizeof arg, stdin);

    // STEP 4: Send the msg
    int numbytes;
    if ((numbytes = sendto(sockfd, arg, strlen(arg), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr_in))) == -1)
    {
        perror("sendto");
        exit(1);
    }

    // STEP 5: Inform the client
    printf("sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));

    // Step 6: Close socket and exit
    close(sockfd);
    return 0;
}