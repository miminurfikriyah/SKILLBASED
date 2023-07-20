#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create a socket for the client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888); // Use the same port as the server
    if (inet_pton(AF_INET, "192.168.46.130", &server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Receive the random number from the server
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Receiving data failed");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[bytes_received] = '\0';

    // Print the received random number
    printf("Received random number: %s\n", buffer);

    // Close the client socket
    close(client_socket);

    return 0;
}
