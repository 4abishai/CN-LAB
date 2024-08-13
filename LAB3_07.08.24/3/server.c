#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVERPORT 4952
#define MAXBUFLEN 200

// Function to perform insertion sort
void insert_sort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    // Create socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Innit server address
    struct sockaddr_in serv_addr, cli_addr;
    memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVERPORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to server addr
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof serv_addr) == -1)
    {
        perror("bind");
        exit(1);
    }

    printf("Server is running and waiting for data...\n");

    // Create a buffer
    char buf[MAXBUFLEN];

    // Receive buffer from the client
    int numbytes;
    socklen_t addr_len = sizeof cli_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                             (struct sockaddr *)&cli_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }
    buf[numbytes] = '\0';
    printf("Got packet from %s\n", inet_ntoa(cli_addr.sin_addr));
    printf("Packet contains \"%s\"\n", buf);

    // Store the buffer into array
    int size;
    int *arr;
    char *token = strtok(buf, " ");
    size = atoi(token);
    arr = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        token = strtok(NULL, " ");
        arr[i] = atoi(token);
    }

    // Sort the array
    insert_sort(arr, size);

    // Store the sorted array to the buffer
    memset(buf, 0, MAXBUFLEN);
    for (int i = 0; i < size; i++)
    {
        char tmp[20];
        sprintf(tmp, "%d ", arr[i]);
        strcat(buf, tmp);
    }

    // Send the buffer
    if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
                           (struct sockaddr *)&cli_addr, addr_len)) == -1)
    {
        perror("sendto");
        exit(1);
    }
    printf("Sent sorted array back to client\n");

    // Free the dynamically allocated memory
    free(arr);

    // Close the socket
    close(sockfd);
    return 0;
}