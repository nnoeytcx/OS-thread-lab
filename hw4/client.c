#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12056
#define PORT2 22056
#define BUFFER_SIZE 1024

int port2()
{
    int sock = 0;
    char buffer[BUFFER_SIZE] = {0};
    char *bye_mes = "Bye";
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT2);

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

    // Send name to server
    // send(sock, buffer, strlen(buffer), 0);
    // Receive greeting message from server
    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);

    snprintf(buffer, sizeof(buffer), "%s", bye_mes);
    send(sock, buffer, strlen(buffer), 0);
    printf("%s\n", bye_mes);
    close(sock);
}
int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *firstname = "Enter your firstname: ";
    char *lastname = "Enter your lastname: ";
    char *birthday = "Enter your birthday in format <ddmmyyyy>: ";
    char buffer[BUFFER_SIZE] = {0};

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

    printf("%s", firstname);
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

    // Send name to server
    send(sock, buffer, strlen(buffer), 0);

    // Receive greeting message from server
    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);

    printf("%s", lastname);
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

    // Send name to server
    send(sock, buffer, strlen(buffer), 0);

    // Receive greeting message from server
    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);

    printf("%s", birthday);
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

    // Send name to server
    send(sock, buffer, strlen(buffer), 0);

    // Receive greeting message from server
    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    close(sock);
    port2();
    
    return 0;
}