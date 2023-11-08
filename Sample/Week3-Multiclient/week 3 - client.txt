#include<stdio.h>                                                                                                                                                       
#include<sys/types.h>                                                                                                                                                   
#include<netinet/in.h>                                                                                                                                                  
#include<sys/socket.h>                                                                                                                                                  
#include<string.h>                                                                                                                                                      
#include<unistd.h>                                                                                                                                                      
int main()                                                                                                                                                              
{                                                                                                                                                                       
        int sockdesc;                                                                                                                                                   
        struct sockaddr_in medaddr;                                                                                                                                     
        sockdesc=socket(AF_INET,SOCK_DGRAM,0);                                                                                                                          
        if(sockdesc==-1)                                                                                                                                                
        {                                                                                                                                                               
                printf("Socket not created");                                                                                                                           
                return -1;                                                                                                                                              
        }                                                                                                                                                               
                                                                                                                                                                        
        medaddr.sin_family=AF_INET;                                                                                                                                     
        medaddr.sin_port=htons(40011);                                                                                                                                  
        medaddr.sin_addr.s_addr=htonl(INADDR_ANY);                                                                                                                      
        char input[30]="";                                                                                                                                              
        char space=' ';                                                                                                                                                 
        printf("Enter the command-");                                                                                                                                   
        scanf("%s",&input);                                                                                                                                             
        scanf("%c",&space);                                                                                                                                             
        int output[1]={0};                                                                                                                                              
        sendto(sockdesc,input,strlen(input),0,(struct sockaddr *)&medaddr,sizeof(medaddr));                                                                             
        int len=sizeof(medaddr);                                                                                                                                        
        recvfrom(sockdesc,output,sizeof(output),0,(struct sockaddr *)&medaddr,&len);                                                                                    
        printf("Message from server: %d", output[0]);                                                                                                                   
        close(sockdesc);                                                                                                                                                
        return 0;                                                                                                                                                       
                                                                                                                                                                        
}                        