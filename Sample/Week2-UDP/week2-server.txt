#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int isPalindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0; // Not a palindrome
        }
    }
    return 1; // Palindrome
}

char *evenOdd(int num) {
    return (num % 2 == 0) ? "Even" : "Odd";
}

char *count01(char *str) {
    int count0 = 0, count1 = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '0') {
            count0++;
        } else if (str[i] == '1') {
            count1++;
        } else {
            return "Invalid input";
        }
    }
    if (count0 == 0 && count1 == 0) {
        return "Neither 0 nor 1 present";
    }
    char *result = malloc(BUFFER_SIZE);
    snprintf(result, BUFFER_SIZE, "Number of 0s: %d, Number of 1s: %d", count0, count1);
    return result;
}

int main() {
    int server_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_addr_len = sizeof(client_address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		perror("bind failed");
        return -1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_addr_len);
        printf("Received command from client: %s\n", buffer);

        char *response = NULL;

        if (strcmp(buffer, "PALINDROME") == 0) {
            char input[BUFFER_SIZE];
            recvfrom(server_fd, input, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_addr_len);
            response = isPalindrome(input) ? "Palindrome" : "Not a palindrome";
        } else if (strcmp(buffer, "EVENODD") == 0) {
            int num;
            recvfrom(server_fd, &num, sizeof(num), 0, (struct sockaddr *)&client_address, &client_addr_len);
            response = evenOdd(num);
        } else if (strcmp(buffer, "NUM01") == 0) {
            char input[BUFFER_SIZE];
            recvfrom(server_fd, input, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_addr_len);
            response = count01(input);
        } else {
            response = "Invalid command";
        }

        // Send response to client
        sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_address, client_addr_len);
    }

    return 0;
}
