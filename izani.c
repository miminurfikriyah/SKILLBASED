#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

// Global variable to track program termination
int terminate_program = 0;

// Signal handler for program termination
void sigintHandler(int signum)
{
    terminate_program = 1;
    printf("\nProgram is terminated by Client.\n");
    exit(0);
}

// Function to get the current time as a formatted string
char *getCurrentTime()
{
    time_t rawTime;
    struct tm *timeInfo;
    char *buffer = malloc(sizeof(char) * 100);
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(buffer, 100, "%d %B %Y, %H:%M", timeInfo);
    return buffer;
}

void handle_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigintHandler);

    char server_domain[] = "izani.synology.me";
    portno = 8443;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        handle_error("Error opening socket");

    server = gethostbyname(server_domain);
    if (server == NULL)
        handle_error("Error, no such host");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        handle_error("Error connecting to server");

    printf("Connected to server successfully!\n");

    char message[256];
    printf("Enter your message: ");
    fgets(message, sizeof(message), stdin);

    n = write(sockfd, message, strlen(message));
    if (n < 0)
        handle_error("Error writing to socket");

    printf("Message sent successfully!\n");

    // Receive and display the server response
    char response[256];
    n = read(sockfd, response, sizeof(response) - 1);
    if (n < 0)
        handle_error("Error reading from socket");

    response[n] = '\0'; // Null-terminate the received response

    printf("Server response: %s\n", response);

    close(sockfd);
    return 0;
}

