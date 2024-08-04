#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 4952 // the port users will be connecting to
#define MAXBUFLEN 200

int main()
{
    // STEP 1: Create UDP socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // STEP 2: Store client's address information
    struct sockaddr_in my_addr;                              // Client's addr info
    struct sockaddr_in their_addr;                           // Server's addr info
    my_addr.sin_family = AF_INET;                            // Address family (byte order)
    my_addr.sin_port = htons(MYPORT);                        // Server's Port converted to network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;                    // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero); // Zero the rest of the struct

    // STEP 3: Bind Socket to Address
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1)
    {
        perror("bind");
        exit(1);
    }

    // STEP 4: Receive the msg into buffer
    int numbytes;
    char buf[MAXBUFLEN];
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0, (struct sockaddr *)&their_addr, &(socklen_t){sizeof(their_addr)})) == -1)
    {
        perror("recvfrom");
        exit(1);
    }

    // Step 5 : Print received data
    printf("got packet from %s\n", inet_ntoa(their_addr.sin_addr));
    printf("packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("packet contains \"%s\"\n", buf);

    // Step 6: Close socket and exit
    close(sockfd);
    return 0;
}