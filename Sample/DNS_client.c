#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
int main()
{
        int client_socket;
        client_socket=socket(AF_INET,SOCK_DGRAM,0);
        if(client_socket<0)
        {
                printf("[-]Socket not created successfully\n");
                exit(1);
        }
        printf("\nSocket created successfully");


        struct sockaddr_in cliaddr;
        char *ip="127.0.0.1";
        int port=6789;
        memset(&cliaddr,'\0',sizeof(cliaddr));
        cliaddr.sin_family=AF_INET;
        cliaddr.sin_port=port;
        cliaddr.sin_addr.s_addr=inet_addr(ip);


        printf("Enter the domain name:");
        char domain[100];
        scanf("%s",domain);

        //Sending the domain name to Local server
        sendto(client_socket,domain,sizeof(domain),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));

        char ipaddr[100];
        //Receiving IP address from Local server
        socklen_t addrsize=sizeof(cliaddr);
        recvfrom(client_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&cliaddr,&addrsize);
        printf("The IP address of the domain %s is: %s",domain,ipaddr);
        close(client_socket);
        return 0;
}
