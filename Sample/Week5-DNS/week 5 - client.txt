#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<time.h>
#define PORT 8081
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define N 100
#include<time.h>

char coms[20][20];
char orgs[20][20];
char ins[20][20];
int main() {
        clock_t tStart = clock();
        int client_fd;
    struct sockaddr_in server_address;
    char input[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};
    strcpy(coms[0],"google");
    strcpy(coms[1],"openai");
    strcpy(coms[2],"instagram");
    strcpy(orgs[0],"wikepedia");
    strcpy(orgs[1],"nationalgeographic");
    strcpy(orgs[2],"cartoonnetwork");
    strcpy(ins[0],"indiangovt");
    strcpy(ins[1],"tngovt");
    strcpy(ins[2],"worldbank");



    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connection failed");
        return -1;
    }
    int c=1;
    printf("Enter choice.\n1 for .org\n2 for .in\n3 for .com: ");
    //scanf("%d",&c);
    send(client_fd, &c,sizeof(c),0);
    int port;
    read(client_fd,&port,sizeof(port));
if(!port){
            printf("Port not found \n");
            return 0;
    }
    else
            printf("Port found : %d \n",port);
    close(client_fd);
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

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
    printf("Enter choice \n");
    if(port == 8082){
        for(int i=0;i<3;i++){
                printf("%d for %s\n",i+1,orgs[i]);
        }
    }
    else if(port == 8083){
        for(int i=0;i<3;i++){
                printf("%d for %s\n",i+1,ins[i]);
        }
    }
    else{
        for(int i=0;i<3;i++){
                printf("%d for %s\n",i+1,coms[i]);
        }
    }
   // scanf("%d",&c);
    char ip[100];
    send(client_fd, &c,sizeof(c),0);
    read(client_fd, ip, sizeof(ip));
    printf("IP address received : %s \n",ip);
    close(client_fd);
    double t=(double)(clock() - tStart) / CLOCKS_PER_SEC;
    printf("Time taken (in seconds) : %f", t);

    return 0;
}
