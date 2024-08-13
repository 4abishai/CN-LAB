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
    // Create UDP socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Create the buffer
    char buf[MAXBUFLEN];
    memset(buf, 0, MAXBUFLEN);

    // Fill in the buffer
    int size;
    printf("Enter array size: ");
    scanf("%d", &size);

    sprintf(buf, "%d ", size); // Put the size at the beginning of the buffer

    printf("Enter array elements: \n");
    for (int i = 0; i < size; i++)
    {
        int elem;
        scanf("%d", &elem);
        char tmp[20];
        sprintf(tmp, "%d ", elem);
        strcat(buf, tmp);
    }

    // Init server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVERPORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send buffer to the server
    int numbytes;
    socklen_t addr_len = sizeof serv_addr;
    if ((numbytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serv_addr, addr_len)) == -1)
    {
        perror("sendto");
        exit(1);
    }
    printf("Sent %d bytes to %s\n", numbytes, inet_ntoa(serv_addr.sin_addr));

    // Receive buffer from the server
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                             (struct sockaddr *)&serv_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }
    buf[numbytes] = '\0';

    printf("Received sorted array: %s\n", buf);

    // Close the socket
    close(sockfd);
    return 0;
}