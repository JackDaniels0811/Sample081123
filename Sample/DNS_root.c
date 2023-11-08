#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
int main()
{
        int root_ser_socket; //Root acting as server - Socket ID
         //Root acting as client - Socket ID
        root_ser_socket=socket(AF_INET,SOCK_DGRAM,0);
        if(root_ser_socket<0)
        {
                printf("[-]Socket not created successfully\n");
                exit(1);
        }
        printf("\nSocket created successfully");


        struct sockaddr_in root_ser_addr;
        char *ip="127.0.0.1";
        int port_ser=6790;
        memset(&root_ser_addr,'\0',sizeof(root_ser_addr));
        root_ser_addr.sin_family=AF_INET;
        root_ser_addr.sin_port=port_ser;
        root_ser_addr.sin_addr.s_addr=inet_addr(ip);

        int n = bind(root_ser_socket,(struct sockaddr*)&root_ser_addr,sizeof(root_ser_addr));
        if(n<0)
        {
                printf("[-]Not binded successfully\n");
                exit(1);
        }
        printf("\nBinded successfully");
        char ipaddr[100];
        while(1)
        {
        struct sockaddr_in local_cli_addr;
        char domain[100];
        socklen_t size = sizeof(local_cli_addr);
        recvfrom(root_ser_socket,domain,sizeof(domain),0,(struct sockaddr*)&local_cli_addr,&size);
        printf("\nThe Domain name received from the Local is:%s",domain);


        int i=0;
        while(domain[i]!='.')i++;
        i++;
        while(domain[i]!='.')i++;
        i++;
        char tldname[10];
        int k=0;
        while(domain[i]!='\0')
                tldname[k++]=domain[i++];
        tldname[k]='\0';

        if(strcmp(tldname,"com")==0)
        {
                int root_cli_socket;
                root_cli_socket=socket(AF_INET,SOCK_DGRAM,0);
                if(root_cli_socket<0)
                {
                        printf("\nSocket not created sucessfully");
                        exit(1);
                }
                printf("\nSocket created successfully");

                int port_cli=6791;
                struct sockaddr_in root_cli_addr;
                memset(&root_cli_addr,'\0',sizeof(root_cli_addr));
                root_cli_addr.sin_family=AF_INET;
                root_cli_addr.sin_port=port_cli;
                root_cli_addr.sin_addr.s_addr=inet_addr(ip);

                socklen_t size=sizeof(root_cli_addr);
                sendto(root_cli_socket,domain,sizeof(domain),0,(struct sockaddr*)&root_cli_addr,sizeof(root_cli_addr));

                recvfrom(root_cli_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&root_cli_addr,&size);
        }

 if(n<0)
        {
                printf("[-]Not binded successfully\n");
                exit(1);
        }
        printf("\nBinded successfully");

        sendto(root_ser_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&local_cli_addr,sizeof(local_cli_addr));


        }
        close(root_ser_socket);
        return 0;
}

