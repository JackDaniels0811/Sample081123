#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERVER_IP "127.0.0.1"

#define PORT 8083
#define AUTH_PORT 8085
#define BUFFER_SIZE 1024
#define N 100

char urls[20][20];
char cache[20][20];

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    strcpy(urls[0],"indiangovt");
    strcpy(urls[1],"tngovt");
    strcpy(urls[2],"worldbank");
    for(int i=0;i<3;i++)
            strcpy(cache[i],"empty");

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
 int input;
        int p=0;
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
 }

        // Read data from the client
        read(new_socket, &input,sizeof(input));
        printf("URL Received from client : %s \n",urls[input-1]);
        char ip[100];
        if(input>3){
                printf("Invalid input \n");
                strcpy(ip,"IP not found");
                send(new_socket,ip,sizeof(ip),0);
        }
        else{
                if(!strcmp(cache[input-1],"empty")){
                int client_fd;
                struct sockaddr_in server_address;
                if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                        perror("socket creation error");
                        return -1;
                }

                server_address.sin_family = AF_INET;
                server_address.sin_port = htons(AUTH_PORT);

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
                char url[100];
                strcpy(url,urls[input-1]);
                strcat(url,".in");
                printf("URL sent : %s \n",url);
                send(client_fd,url,sizeof(url),0);
                int f;
                read(client_fd,ip,sizeof(ip));
                read(client_fd,&f,sizeof(f));
                strcpy(cache[input-1],ip);
                printf("Added to cache %s\n",cache[input-1]);

                close(client_fd);
                send(new_socket,ip,sizeof(ip),0);
                }
                 else{
                        printf("Retrieved from cache \n");
                        send(new_socket,cache[input-1],sizeof(cache[input-1]),0);
 }

        }
        close(new_socket);
    }

    return 0;
}
