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
struct student{
    int regno;
    int ncourses;
    char courses[20][20];
    int marks[20];
};

typedef struct{
 char method[N];
 char path[N];
 char version[N];
 char accept[N];
 char connection[N];
 char useragent[N];
}request;

typedef struct{
 int status;
 char statmsg[N];
 char version[N];
 char connection[N];
 char contype[N];
}response;

int main() {
    clock_t tStart = clock();
    int client_fd;
    struct sockaddr_in server_address;
    char input[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};
    request cr;
    strcpy(cr.method, "GET /marks/index.html");
    strcpy(cr.path, "www.students.com");
    strcpy(cr.version, "http/1.1");
    strcpy(cr.connection, "keep-alive");
    strcpy(cr.accept, "text");
    strcpy(cr.useragent,"Mozilla/5.0");
    // Create socket
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

    printf("Enter Register number to send to the server: ");
    int regno;
    scanf("%d", &regno);
    send(client_fd, &regno,sizeof(regno),0);
    send(client_fd, &cr, sizeof(cr), 0);
    response res;
    char courses[20][20];
    int ncourses;
    read(client_fd,&ncourses,sizeof(ncourses));
    read(client_fd, &courses, sizeof(courses));
    read(client_fd, &res, sizeof(response));
    printf("Courses received from server: \n");
    for(int i=0;i<ncourses;i++){
        printf("%s\n",courses[i]);
    }
    printf("%s\n %d\n",res.statmsg,res.status);
    for(int i=0;i<ncourses;i++){
    int cno = i;
    request cr1;
    response res1;
    strcpy(cr1.method, "GET /marks/");
    strcat(cr1.method,courses[cno]);
    cr1.method[strlen(cr1.method)] = '\0';
    strcpy(cr1.path, "www.students.com");
    strcpy(cr1.version, "http/1.1");
    strcpy(cr1.connection, "keep-alive");
    strcpy(cr1.accept, "text");
    strcpy(cr1.useragent,"Mozilla/5.0");
    send(client_fd,&cno,sizeof(cno),0);
    send(client_fd,&cr1,sizeof(cr1),0);
    int marks;
    read(client_fd,&marks,sizeof(marks));
    read(client_fd,&res1,sizeof(res1));
    printf("Marks received from server : %d \n",marks);
    printf("%s\n %d\n",res1.statmsg,res1.status);
    }
    close(client_fd);
    double t=(double)(clock() - tStart) / CLOCKS_PER_SEC;
    printf("Time taken (in seconds) : %f", t);
    return 0;
}
