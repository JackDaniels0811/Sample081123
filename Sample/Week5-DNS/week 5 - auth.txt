#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8085
#define BUFFER_SIZE 1024
#define N 100

char urls[40][40];
char ips[20][20];

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    strcpy(urls[0],"google.com");
    strcpy(urls[1],"openai.com");
    strcpy(urls[2],"instagram.com");
    strcpy(urls[3],"wikepedia.org");
    strcpy(urls[4],"nationalgeographic.org");
    strcpy(urls[5],"cartoonnetwork.org");
    strcpy(urls[6],"indiangovt.in");
    strcpy(urls[7],"tngovt.in");
    strcpy(urls[8],"worldbank.in");
    strcpy(ips[0],"1.1.1.1");
    strcpy(ips[1],"2.2.2.2");
    strcpy(ips[2],"3.3.3.3");
    strcpy(ips[3],"4.4.4.4");
    strcpy(ips[4],"5.5.5.5");
    strcpy(ips[5],"6.6.6.6");
    strcpy(ips[6],"7.7.7.7");
    strcpy(ips[7],"8.8.8.8");
    strcpy(ips[8],"9.9.9.9");

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
        char ip[100];
        int ind=-1,f=0;
        char ret[100];
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        read(new_socket, ip,sizeof(ip));
        printf("URL Received from client : %s \n",ip);
        for(int i=0;i<9;i++)
                if(!strcmp(ip,urls[i]))
                                ind = i;
        if(ind==-1){
                printf("URL not found \n");
                strcpy(ret,"Not found");
                send(new_socket,ret,sizeof(ret),0);
                send(new_socket,&f,sizeof(f),0);
        }
        else{
                f = 1;
                strcpy(ret,ips[ind]);
                send(new_socket,ret,sizeof(ret),0);
                send(new_socket,&f,sizeof(f),0);
        }
        close(new_socket);
    }

    return 0;
}
