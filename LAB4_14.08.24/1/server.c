/*
            NETWORK PROGRAMMING WITH SOCKETS

We show the communication between a server
process and a client process.

Since many server processes may be running in a system, we identify the
desired server process by a "port number". Standard server processes have
a worldwide unique port number associated with it. For example, the port
number of SMTP (the sendmail process) is 25. To see a list of server
processes and their port numbers see the file /etc/services

In this program, we choose port number 6000 for our server process. Here we
shall demonstrate TCP connections only. For details and for other types of
connections see:

     Unix Network Programming
        -- W. Richard Stevens, Prentice Hall India.

To create a TCP server process, we first need to open a "socket" using the
socket() system call. This is similar to opening a file, and returns a socket
descriptor. The socket is then bound to the desired port number. After this
the process waits to "accept" client connections.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#define SERVPORT 3000

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

    // Fill the buffer
    char buf[100];
    memset(buf, '\0', 100);
    // strcpy(buf, "Message from server");
    sprintf(buf, "%s", "Message from server");

    // Send the buffer
    send(cli_sock, buf, 100, 0);

    // Receive messag into buffer
    memset(buf, '\0', 100);
    recv(cli_sock, buf, 100, 0);

    printf("Received: %s\n", buf);

    close(cli_sock);
    close(serv_sock);
    return 0;
}