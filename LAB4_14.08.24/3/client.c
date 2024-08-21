#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXBUFLEN 100
#define SERVPORT 3550

int main()
{

    int sockfd;
    // Create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket faulted");
        close(sockfd);
        exit(1);
    }

    // Configure server address
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERVPORT);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&sa, sizeof sa) == -1)
    {
        perror("Connect faulted");
        close(sockfd);
        exit(1);
    }

    // Get command from the user
    printf("> ");
    char buf[MAXBUFLEN] = {0};
    fgets(buf, sizeof buf, stdin);

    // Send cmd buffer to the server
    if (send(sockfd, buf, MAXBUFLEN, 0) == -1)
    {
        perror("Send faulted");
        close(sockfd);
        exit(1);
    }

    // Receive listing buffer
    memset(buf, 0, MAXBUFLEN);
    if (recv(sockfd, buf, MAXBUFLEN, 0) == -1)
    {
        perror("Recv faulted");
        close(sockfd);
        exit(1);
    }

    printf("\n%s", buf);

    close(sockfd);
    return 0;
}