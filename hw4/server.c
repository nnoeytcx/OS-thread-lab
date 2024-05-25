#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12056
#define PORT2 22056
#define BUFFER_SIZE 1024

int	getlen 	(int num)
{
	int len = 0;

	if (num < 0)
		len++;
	while (num != 0)
	{
		len++;
		num /= 10;
	}
	return (len);
}

char	*ft_itoa(int nbr)
{
	char	*res;
	int	len;
	long	num = nbr;

	len = getlen(nbr);
	res = malloc(sizeof(char) * (len + 1));
	res[len] = '\0';

	if (num < 0)
		res[0] = '-';
	else if (num == 0)
		res[0] = '0';

	while (num != 0)
	{
		--len;
		if (num < 0)
			num *= -1;
		res[len] = (num%10) + '0';
		num /= 10;
	}
	return (res);
}
void port2(char *buffer)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *recieved_mes = "Bye";
    char response[BUFFER_SIZE];
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the network address and port
    bzero((char *)&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT2);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT2);

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // printf("time is %d", time);

    // Read client's name
    //read(new_socket, buffer, BUFFER_SIZE);
    printf("Year is : %s\n", buffer);

    // Send recieved_mes message to client
    snprintf(response, sizeof(response), "%s", buffer);
    send(new_socket, response, strlen(response), 0);

    bzero(buffer, BUFFER_SIZE);
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Client's response: %s\n", buffer);
    close(new_socket);
    close(server_fd);
}
int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int year = 0;
    int time = 3;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *recieved_mes = "Server received";
    char response[BUFFER_SIZE];
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the network address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (time > 0)
    {
        // printf("time is %d", time);

        // Read client's name
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Client's data: %s\n", buffer);

        // Send recieved_mes message to client
        snprintf(response, sizeof(response), "%s", recieved_mes);
        send(new_socket, response, strlen(response), 0);
        if (time > 1)
            bzero(buffer, BUFFER_SIZE);
        time--;
    }
    close(new_socket);
    close(server_fd);

    year = atoi(buffer);
    year%=10000;
    year-=543;
    char *str_year = ft_itoa(year);
    // printf("%s", str_year);
    int i = 0;
    while (i < 4)
    {
        buffer[i] = str_year[i];
        // printf("<%c>", buffer[i]);
        i++;
    }
    buffer[i] = '\0';
    port2(buffer);

    return 0;
}