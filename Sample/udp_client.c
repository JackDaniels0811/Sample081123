#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        printf("Enter text to send (type 'q' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = 0;

        // Send data to server
        sendto(sockfd, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&server_addr, sizeof(server_addr));

        // Receive reversed string from server
        int len = recvfrom(sockfd, (char*)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&server_addr, &(socklen_t){sizeof(server_addr)});
        buffer[len] = '\0';

        printf("Received from server: %s\n", buffer);

        if (strcmp(buffer, "q") == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    return 0;
}