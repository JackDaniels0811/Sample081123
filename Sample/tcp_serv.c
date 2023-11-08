#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 7090

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to a port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == 0) {
        printf("Listening...\n");
    } else {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    while (1) {
        // Receive data from client
        memset(buffer, 0, sizeof(buffer));
        recv(new_sock, buffer, 1024, 0);
        printf("Client: %s\n", buffer);

        // Reverse the message
        int len = strlen(buffer);
        for (int i = 0; i < len / 2; i++) {
            char temp = buffer[i];
            buffer[i] = buffer[len - i - 1];
            buffer[len - i - 1] = temp;
        }

        // Send reversed message back to client
        send(new_sock, buffer, strlen(buffer), 0);
    }

    close(new_sock);
    close(sockfd);

    return 0;
}