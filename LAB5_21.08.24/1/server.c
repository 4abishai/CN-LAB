#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVPORT 3000

int getSumOfDigits(int num)
{
    int sum = 0;
    while (num > 0)
    {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

int main()
{
    // Create TCP socket
    int serv_sock, cli_sock;
    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(1);
    }

    // Configure address
    struct sockaddr_in sa, ca; // server address, client address
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(SERVPORT);

    // Bind the socket with the server address
    if ((bind(serv_sock, (struct sockaddr *)&sa, sizeof sa)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    // Atmost 5 requests can queue up for connection
    if (listen(serv_sock, 5) == -1)
    {
        perror("Listen failed");
        exit(1);
    }
    printf("Server is listening...\n");

    // Accept an incoming connection from a client
    socklen_t length = sizeof ca;
    if ((cli_sock = accept(serv_sock, (struct sockaddr *)&ca, &length)) == -1)
    {
        perror("Accept failed");
        exit(1);
    }

    // Receive message into buffer
    char buf[100];
    memset(buf, '\0', 100);
    recv(cli_sock, buf, 100, 0);

    // Scan number into buffer
    int num;
    sscanf(buf, "%d", &num);

    // Prepare the buffer
    memset(buf, '\0', 100);
    sprintf(buf, "%d", getSumOfDigits(num));

    // Send the buffer
    send(cli_sock, buf, 100, 0);

    close(cli_sock);
    close(serv_sock);
    return 0;
}