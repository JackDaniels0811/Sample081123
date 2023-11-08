#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 7090

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char input[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Get input from user
        printf("Enter message: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // remove newline character

        // Send data to server
        send(sockfd, input, strlen(input), 0);

        // Receive data from server
        memset(buffer, 0, sizeof(buffer));
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("Server: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}