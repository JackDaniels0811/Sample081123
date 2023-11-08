#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_address;
    char command[BUFFER_SIZE];
    char input[BUFFER_SIZE];
    int num;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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

    while (1) {
        printf("Enter command (PALINDROME, EVENODD, NUM01): ");
        scanf("%s", command);

        // Send command to server
        sendto(client_fd, command, strlen(command), 0, (struct sockaddr *)&server_address, sizeof(server_address));

        if (strcmp(command, "PALINDROME") == 0) {
            printf("Enter a string: ");
            scanf("%s", input);

            // Send input string to server
            sendto(client_fd, input, strlen(input), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        } else if (strcmp(command, "EVENODD") == 0) {
            printf("Enter a number: ");
            scanf("%d", &num);

            // Send number to server
            sendto(client_fd, &num, sizeof(num), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        } else if (strcmp(command, "NUM01") == 0) {
            printf("Enter a string containing 0s and 1s: ");
            scanf("%s", input);

            // Send input string to server
            sendto(client_fd, input, strlen(input), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        }
		// Receive and display response from server
        char response[BUFFER_SIZE];
        int len = sizeof(server_address);
        recvfrom(client_fd, response, BUFFER_SIZE, 0, NULL, NULL);
        printf("Server response: %s\n", response);
    }

    close(client_fd);

    return 0;
}
