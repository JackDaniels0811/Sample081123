#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
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
void read_objects_from_file(const char* filename, struct student people[], int count) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Unable to open file for reading");
        return;
    }

    fread(people, sizeof(struct student), count, file);

    fclose(file);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    response sr;
    strcpy(sr.version, "http/1.1");
    strcpy(sr.connection, "keep-alive");
    strcpy(sr.contype, "text");
    char courses[20][20];
    int marks[20];
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


        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        request req;
        // Read data from the client
        int regno;
        read(new_socket, &regno,sizeof(regno));
        printf("Register number received from client : %d \n",regno);
        read(new_socket, &req, sizeof(request));

        printf("%s \n%s \n%s \n",req.connection,req.version,req.method);
        struct student students[3];
        const char* filename = "students.bin";
        read_objects_from_file(filename, students, 3);
        int flag = 0;
        int ncourses = 0;
        for(int i=0;i<3;i++){
                if(regno == students[i].regno){
                        flag = 1;
                        for(int j=0;j<students[i].ncourses;j++){
                                strcpy(courses[j],students[i].courses[j]);
                                marks[j] = students[i].marks[j];
                                ncourses = students[i].ncourses;
                        }
                        strcpy(sr.statmsg,"Found");
						sr.status = 200;
                }
        }
        if(!flag){
                strcpy(sr.statmsg, "Not found");
                sr.status = 404;
        }
        send(new_socket, &ncourses, sizeof(ncourses),0);
        send(new_socket, courses, sizeof(courses), 0);
        send(new_socket, &sr, sizeof(sr), 0);
        printf(" data sent back to client\n");
        close(new_socket);

for(int i=0;i<ncourses;i++){
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    int port = 8082 + i;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

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

        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        request req1;
        response sr1;
        strcpy(sr1.version, "http/1.1");
        strcpy(sr1.connection, "keep-alive");
        strcpy(sr1.contype, "text");
        int index = 0;
        read(new_socket,&index,sizeof(index));
        printf("Course received from client : %s \n",courses[index]);
        read(new_socket,&req1,sizeof(request));
		printf("%s \n%s \n%s \n",req1.connection,req1.version,req1.method);
        strcpy(sr1.statmsg,"Found");
        sr1.status = 200;
        send(new_socket, &marks[index],sizeof(marks[index]),0);
        send(new_socket,&sr1,sizeof(sr1),0);
        close(new_socket);
}
    return 0;
}
