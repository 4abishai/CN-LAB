#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXBUFLEN 100
#define SERVPORT 3000

int main()
{
    // Create TCP socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); // Loop back IP address
    sa.sin_port = htons(SERVPORT);

    // Connect with the server
    if (connect(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    {
        perror("Connection failed");
        exit(1);
    }

    char buf[MAXBUFLEN];
    memset(buf, '\0', MAXBUFLEN);

    // Get the number
    int num;
    printf("Enter number: ");
    scanf("%d", &num);
    sprintf(buf, "%d", num);

    // Send the buffer
    send(sockfd, buf, strlen(buf), 0);

    int numbytes = recv(sockfd, buf, MAXBUFLEN, 0);
    printf("Received: %s\n", buf);

    printf("Received %d bytes\n", numbytes);
    printf("IP address: %s", inet_ntoa(sa.sin_addr));

    close(sockfd);
    return 0;
}