#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MYPORT 4952
#define MAXBUFLEN 200

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addr_len;
    int numbytes;
    char buf[MAXBUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MYPORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof serv_addr) == -1)
    {
        perror("bind");
        exit(1);
    }

    addr_len = sizeof cli_addr;
    printf("Server is running and waiting for data...\n");

    while (1)
    {
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                                 (struct sockaddr *)&cli_addr, &addr_len)) == -1)
        {
            perror("recvfrom");
            exit(1);
        }

        buf[numbytes] = '\0';
        printf("Received: \"%s\"\n", buf);

        // Send the same packet back to the client
        if (sendto(sockfd, buf, numbytes, 0,
                   (struct sockaddr *)&cli_addr, addr_len) == -1)
        {
            perror("sendto");
            exit(1);
        }

        if (strcmp(buf, "exit") == 0)
            break;
    }

    close(sockfd);
    return 0;
}