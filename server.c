#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define NUM_THREADS     10
#define PORT 12056
#define BUFFER_SIZE 1024
#define WIDTH 40 
#define HEIGHT 10 
#define PLAYER 'P' 
#define WALL '#' 
#define COIN 'C' 
#define SPACE ' ' 
#define ENERMY 'X' 

pthread_mutex_t print_mutex;
int score = 0; 
int coin = 25;
int k = 0;
int player_x = 11, player_y = 3; 
char board[HEIGHT][WIDTH];
char *map[HEIGHT] = 
{"########################################",
"#        C #  C   #  ## ##      #    #C#",
"#     X        X    #     ##           #",
"#   #      P     C  C       #    ##C#C #",
"#                  C #      #   XC     #",
"##   #   #  C     C#   C    C   X  # CX#",
"#        C    X#C# ##X   # # C      #  #",
"#X   #    #   X  # #  X  CC # C#   C#  #",
"#  #    C #C      #    # C         #  C#",
"########################################"}; 
void assignmap()
{
    for (int i=0 ; i<HEIGHT ;i++)
    {
        for (int j=0 ; j<WIDTH ;j++)
        {
            board[i][j] = (char)map[i][j];
        }
    }
}
void *print(void *threadid)
{
    int *id_ptr, taskid;

    id_ptr = (int *)threadid;
    taskid = *id_ptr;
    pthread_mutex_lock(&print_mutex);
    printf("Thread %d:", taskid);
    int i = 0;
    while (i < WIDTH)
    {
        printf("%c", board[k][i]);
        i++;
    }
    printf("\n");
    k++;
    if (k == 10)
        k = 0;
    pthread_mutex_unlock(&print_mutex);
    pthread_exit(NULL);
}

void th_print()
{
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t;
    pthread_mutex_init(&print_mutex, NULL);

    for (t = 0; t < NUM_THREADS; t++)
    {
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        // printf("Creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, print, (void *)taskids[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
}
int	getlen(int num)
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
int move(char command)
{
    int move_x = 0; 
    int move_y = 0;

    if (command == 'W' || command == 'w')
        move_y -= 1;
    if (command == 'A' || command == 'a')
        move_x -= 1;
    if (command == 'S' || command == 's')
        move_y += 1;
    if (command == 'D' || command == 'd')
        move_x += 1;
    int x = player_x + move_x; 
    int y = player_y + move_y; 
    if (board[y][x] && board[y][x] != WALL) { 
        if (board[y][x] == COIN) { 
            score++; 
            coin--;
            if (coin == 0)
                return (2);
        } 
        else if (board[y][x] == ENERMY) { 
            return (0);
        } 
  
        board[player_y][player_x] = SPACE; 
        player_x = x; 
        player_y = y; 
        board[player_y][player_x] = PLAYER; 
    } 
    return (1);
}
void printmap()
{
    int i = 0,j;
    while (i < HEIGHT)
    {
        j = 0;
        while (j < WIDTH)
        {
            printf("%c", board[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}
int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *dead = "You are dead.";
    char *bye = "Bye, Thanks.";
    char *win = "Yay, You win.";
    assignmap();
    char buffer[BUFFER_SIZE] = {0};
    int status = 1;
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
    printf("+++WELCOME TO PACMAN GAME+++\n");
    th_print();
    // printmap();

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    while (buffer[0] != 'X' )
    {
        read(new_socket, buffer, BUFFER_SIZE);
        if (buffer[0] == 'X')//recieve 'exit' from client
        {
            send(new_socket, bye, strlen(bye), 0);
            break;
        }
        
        printf("The move is: <%s>\n", buffer);
        status = move(buffer[0]);
        if (!status)//player colide with enermy
        {
            send(new_socket, dead, strlen(dead), 0);
            printf("%s\n", dead);
            break;
        }
        else if (status == 2)//player collect all coin
        {
            send(new_socket, win, strlen(win), 0);
            printf("%s\n", win);
            break;
        }
        // printmap();
        th_print();

        //send score
        send(new_socket, ft_itoa(score), strlen(ft_itoa(score)), 0);
        bzero(buffer, BUFFER_SIZE);
    }
    printf("Thanks for plaing with me ^_^\n");
    close(new_socket);
    close(server_fd);
    pthread_mutex_destroy(&print_mutex);
    pthread_exit(NULL);
    return 0;
}