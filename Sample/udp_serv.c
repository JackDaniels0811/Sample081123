#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to server address
    if (bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    while (1) {
        // Receive data from client
        int len = recvfrom(sockfd, (char*)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&client_addr, &client_len);
        buffer[len] = '\0';

        printf("Received from client: %s\n", buffer);

        // Reverse the string
        int i, j;
        char temp;
        for (i = 0, j = strlen(buffer) - 1; i < j; i++, j--) {
            temp = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = temp;
        }

        // Send reversed string back to client
        sendto(sockfd, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&client_addr, client_len);

        printf("Reversed string sent to client: %s\n", buffer);
    }

    return 0;
}