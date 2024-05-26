#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12056
#define BUFFER_SIZE 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *move = "Enter your move <W A S D or X to exit> : ";
    char buffer[BUFFER_SIZE] = {0};
    char mode = 'y';

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (mode != 'X')
    {
        printf("%s", move);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        // Send action to server
        send(sock, buffer, strlen(buffer), 0);
        bzero(buffer, BUFFER_SIZE);

        // Receive status message from server
        read(sock, buffer, BUFFER_SIZE);
        mode = buffer[0];
        if (mode == 'B' || mode == 'Y')//dead or bye or win
            break;
        printf("The score is : %s\n", buffer);
        bzero(buffer, BUFFER_SIZE);
    }

    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    close(sock);
    return 0;
}