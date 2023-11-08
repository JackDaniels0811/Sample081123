#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

// Function to perform bit stuffing
void performBitStuffing(char *input, char *output) {
    int consecutiveOnes = 0;
    int outputIndex = 0;

    for (int i = 0; i < strlen(input); i++) {
        output[outputIndex++] = input[i];

        if (input[i] == '1') {
            consecutiveOnes++;
            if (consecutiveOnes == 5) {
                output[outputIndex++] = '0'; // Stuff a '0' bit
                consecutiveOnes = 0; // Reset consecutive ones count
            }
        } else {
            consecutiveOnes = 0; // Reset consecutive ones count
        }
    }

    output[outputIndex] = '\0'; // Null-terminate the output string
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
 }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read data from the client
        read(new_socket, buffer, BUFFER_SIZE);

        // Perform bit stuffing
        char stuffedBuffer[BUFFER_SIZE];
        performBitStuffing(buffer, stuffedBuffer);

        // Send the stuffed data back to the client
        send(new_socket, stuffedBuffer, strlen(stuffedBuffer), 0);
        printf("Stuffed data sent back to client: %s\n", stuffedBuffer);

        close(new_socket);
    }

    return 0;
}
