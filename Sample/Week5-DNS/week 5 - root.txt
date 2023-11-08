#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define N 100

char urls[20][20];
int port[20];

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    strcpy(urls[0],"org");
    strcpy(urls[1],"in");
    strcpy(urls[2],"com");
    for(int i=0;i<3;i++){
        port[i] = 8082+i;
    }
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int input;
        int p=0;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read data from the client
        read(new_socket, &input,sizeof(input));
        printf("URL Received from client : %s \n",urls[input-1]);
        if(input>3){
                printf("Port not found \n");
                send(new_socket,&p,sizeof(p),0);
        }
        else{
                p = port[input-1];
                send(new_socket,&p,sizeof(p),0);
        }
        close(new_socket);
    }

    return 0;
}
