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

#define SERVERPORT 4952
#define MAXBUFLEN 200

int main()
{
    int sockfd;
    struct sockaddr_in their_addr;
    int numbytes;
    char arg[MAXBUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(SERVERPORT);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    printf("Enter a message: ");
    gets(arg);

    if ((numbytes = sendto(sockfd, arg, strlen(arg), 0,
                           (struct sockaddr *)&their_addr, sizeof their_addr)) == -1)
    {
        perror("sendto");
        exit(1);
    }
    printf("Sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));

    char buf[MAXBUFLEN];
    socklen_t addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }
    buf[numbytes] = '\0';
    printf("Got packet from %s\n", inet_ntoa(their_addr.sin_addr));
    printf("Packet is %d bytes long\n", numbytes);
    printf("Packet contains \"%s\"\n", buf);

    close(sockfd);
    return 0;
}