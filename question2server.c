#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MIN_RANDOM_NUMBER 100
#define MAX_RANDOM_NUMBER 999
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;
    char buffer[BUFFER_SIZE];

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create the server socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888); // Use any available port

    // Bind the server socket to the specified IP address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8888 ...\n");

    while (1) {
        // Accept a client connection
        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Failed to accept the client connection");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        // Generate a random number between 100 and 999
        int random_number = (rand() % (MAX_RANDOM_NUMBER - MIN_RANDOM_NUMBER + 1)) + MIN_RANDOM_NUMBER;

        // Convert the random number to a string
        snprintf(buffer, BUFFER_SIZE, "%d", random_number);

        // Send the random number to the client
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Failed to send data to the client");
            exit(EXIT_FAILURE);
        }

        printf("Random number sent: %s\n", buffer);

        // Close the client socket
        close(client_socket);
        printf("Client disconnected\n");
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

