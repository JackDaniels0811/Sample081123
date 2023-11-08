#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
int main()
{
        //Tld as server
        int tld_ser_socket;
        struct sockaddr_in tld_ser_addr;
        struct sockaddr_in root_cli_addr;

        tld_ser_socket=socket(AF_INET,SOCK_DGRAM,0);
        if(tld_ser_socket<0)
        {
                printf("[-]Socket not created successfully\n");
                exit(1);
        }
        printf("\nSocket created successfully");


        char *ip="127.0.0.1";
        int port_ser=6791;
        memset(&tld_ser_addr,'\0',sizeof(tld_ser_addr));
        tld_ser_addr.sin_family=AF_INET;
        tld_ser_addr.sin_port=port_ser;
        tld_ser_addr.sin_addr.s_addr=inet_addr(ip);

        int n = bind(tld_ser_socket,(struct sockaddr*)&tld_ser_addr,sizeof(tld_ser_addr));
        if(n<0)
        {
                printf("[-]Not binded successfully\n");
                exit(1);
        }
        printf("\nNot binded successfully");

        while(1)
        {
                socklen_t size=sizeof(root_cli_addr);
                char domain[100];
                char ipaddr[100]="189.2.3.78";
                recvfrom(tld_ser_socket,domain,sizeof(domain),0,(struct sockaddr*)&root_cli_addr,&size);
                printf("The domain obtained is:%s",domain);
                sendto(tld_ser_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&root_cli_addr,sizeof(root_cli_addr));
        }

        close(tld_ser_socket);
        return 0;
}
