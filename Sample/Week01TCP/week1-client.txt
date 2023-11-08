#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_address;
    char input[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("address conversion error");
        return -1;
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connection failed");
        return -1;
    }

    printf("Enter data to send to the server: ");
    scanf("%s", input);

    // Send data to the server
    send(client_fd, input, strlen(input), 0);
    printf("Data sent to server: %s\n", input);

    // Receive stuffed data from the server
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Stuffed data received from server: %s\n", buffer);

    close(client_fd);

    return 0;
}
