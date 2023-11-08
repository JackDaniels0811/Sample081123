#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define LOCAL_PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in local_addr, server_addr;
    socklen_t local_len = sizeof(local_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&local_addr, 0, sizeof(local_addr));
    memset(&server_addr, 0, sizeof(server_addr));

    // Configure local server address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(LOCAL_PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    // Configure server address (localhost for testing)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LOCAL_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    while (1) {
        printf("Enter domain name (type 'q' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "q") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Send domain name to local server
        sendto(sockfd, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&local_addr, sizeof(local_addr));

        // Receive port from local server
        int len = recvfrom(sockfd, (char*)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&local_addr, &local_len);
        buffer[len] = '\0';

        if (strcmp(buffer, "Invalid") == 0) {
            printf("Invalid domain name!\n");
        } else {
            printf("Received port from local server: %s\n", buffer);
        }
    }

    return 0;
}