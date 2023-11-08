#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
int main()
{
        int local_cli_socket;
        int local_ser_socket;
        local_ser_socket=socket(AF_INET,SOCK_DGRAM,0);
        if(local_ser_socket<0)
        {
                printf("[-]Socket not created successfully\n");
                exit(1);
        }
        printf("\nSocket created successfully");


        struct sockaddr_in local_ser_addr;
        char *ip="127.0.0.1";
        int port_ser=6789;
        memset(&local_ser_addr,'\0',sizeof(local_ser_addr));
        local_ser_addr.sin_family=AF_INET;
        local_ser_addr.sin_port=port_ser;
        local_ser_addr.sin_addr.s_addr=inet_addr(ip);

        int n = bind(local_ser_socket,(struct sockaddr*)&local_ser_addr,sizeof(local_ser_addr));
        if(n<0)
        {
                printf("[-]Not binded successfully\n");
                exit(1);
        }
        printf("\nBinded successfully");

        char cachedomain[][100]={};
        char cacheips[][100]={};
        int cnt=0;
        while(1)
        {

                struct sockaddr_in cli_cli_addr;
                socklen_t addrsize=sizeof(cli_cli_addr);
                //Getting the domain name from the client
                char domain[100];
                recvfrom(local_ser_socket,domain,sizeof(domain),0,(struct sockaddr*)&cli_cli_addr,&addrsize);
                printf("\nThe domain received from the client is:%s",domain);
                int flag=0;
                if(cnt!=0)
                {
                        int len=sizeof(cachedomain)/sizeof(cachedomain[0]);
                        for(int i=0;i<len;i++)
                        {
                                if(strcmp(cachedomain[i],domain)==0)
                                {
                                        printf("From cache\n");
                                        flag=1;
                                        char ipaddr[100];
                                        strcpy(ipaddr,"1.1.1.1");
                                        //strcpy(ipaddr,cacheips[i]);
                                        sendto(local_ser_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&cli_cli_addr,sizeof(cli_cli_addr));
                                        close(local_cli_socket);
                                }
                        }
                }
                printf("From cache....\n");
                //Local acting as client
                if(!flag){
                local_cli_socket=socket(AF_INET,SOCK_DGRAM,0);
                if(local_cli_socket<0)
                {
                        printf("[-]Socket not created successfully\n");
                        exit(1);
                }
                printf("\nSocket created successfully");


                struct sockaddr_in local_cli_addr;
                int port_cli=6790;
                memset(&local_cli_addr,'\0',sizeof(local_cli_addr));
                local_cli_addr.sin_family=AF_INET;
                local_cli_addr.sin_port=port_cli;
                local_cli_addr.sin_addr.s_addr=inet_addr(ip);

                //Passing the domain name to root
                sendto(local_cli_socket,domain,sizeof(domain),0,(struct sockaddr*)&local_cli_addr,sizeof(local_cli_addr));

                //Receiving the IP Address from Root server
                char ipaddr[100];
                socklen_t addrsize_ltr=sizeof(local_cli_addr);
                recvfrom(local_cli_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&local_cli_addr,&addrsize_ltr);

                printf("\nThe IP Address received from the Root is:%s",ipaddr);

                //Sending the IP Address to the client
                sendto(local_ser_socket,ipaddr,sizeof(ipaddr),0,(struct sockaddr*)&cli_cli_addr,sizeof(cli_cli_addr));
                if(cnt==0)
                {
                        strcpy(cachedomain[0],domain);
                        strcpy(cacheips[0],ipaddr);
                }
                cnt++;
                }

        }
        close(local_ser_socket);
        close(local_cli_socket);
        return 0;
}
