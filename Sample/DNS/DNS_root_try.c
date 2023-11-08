#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define ROOT_PORT 12346
#define MAX_BUFFER_SIZE 1024

void getPortForDomain(char* domain, char* port) {
    char* extensions[] = {".com", ".edu"};
    char* ports[] = {"7080", "7081"};

    int i;
    for (i = 0; i < 2; i++) {
        if (strstr(domain, extensions[i]) != NULL) {
            strcpy(port, ports[i]);
            return;
        }
    }

    strcpy(port, "Invalid");
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, local_addr;
    socklen_t local_len = sizeof(local_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&local_addr, 0, sizeof(local_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(ROOT_PORT);

    // Bind socket to server address
    if (bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Root Server is running...\n");

    while (1) {
        // Receive data from local server
        int len = recvfrom(sockfd, (char*)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&local_addr, &local_len);
        buffer[len] = '\0';

        printf("Received domain from local server: %s\n", buffer);

        // Get port for domain
        char port[MAX_BUFFER_SIZE];
        getPortForDomain(buffer, port);

        // Send port back to local server
        sendto(sockfd, (const char*)port, strlen(port), MSG_CONFIRM, (const struct sockaddr*)&local_addr, local_len);

        printf("Sent port to local server: %s\n", port);
    }

    return 0;
}