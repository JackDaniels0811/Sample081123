#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define LOCAL_PORT 12345
#define ROOT_PORT 12346
#define MAX_BUFFER_SIZE 1024

int main() {
    int local_sock, root_sock;
    struct sockaddr_in local_addr, root_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP sockets
    if ((local_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 || (root_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&local_addr, 0, sizeof(local_addr));
    memset(&root_addr, 0, sizeof(root_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure local server address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(LOCAL_PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    // Configure root server address
    root_addr.sin_family = AF_INET;
    root_addr.sin_port = htons(ROOT_PORT);
    root_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to local server address
    if (bind(local_sock, (const struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Local Server is running...\n");

    while (1) {
        // Receive data from client
        int len = recvfrom(local_sock, (char*)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&client_addr, &client_len);
        buffer[len] = '\0';

        printf("Received domain from client: %s\n", buffer);

        // Send domain to root server
        sendto(root_sock, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&root_addr, sizeof(root_addr));

        // Receive port from root server
        len = recvfrom(root_sock, (char*)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&root_addr, &client_len);
        buffer[len] = '\0';

        printf("Received port from root server: %s\n", buffer);

        // Send port back to client
        sendto(local_sock, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&client_addr, client_len);

        printf("Sent port to client: %s\n", buffer);
    }

    return 0;
}