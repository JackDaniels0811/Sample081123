#include<stdio.h>                                                                                                                                                       
#include<sys/types.h>                                                                                                                                                   
#include<netinet/in.h>                                                                                                                                                  
#include<sys/socket.h>                                                                                                                                                  
#include<string.h>                                                                                                                                                      
#include<unistd.h>                                                                                                                                                      
int main()                                                                                                                                                              
{                                                                                                                                                                       
        int sockdesc;                                                                                                                                                   
        struct sockaddr_in medaddr,cliaddr,serv1addr,serv2addr;                                                                                                         
                                                                                                                                                                        
        sockdesc=socket(AF_INET,SOCK_DGRAM,0);                                                                                                                          
        if(sockdesc==-1)                                                                                                                                                
        {                                                                                                                                                               
                printf("Socket not created");                                                                                                                           
                return -1;                                                                                                                                              
        }                                                                                                                                                               
                                                                                                                                                                        
        medaddr.sin_family=AF_INET;                                                                                                                                     
        medaddr.sin_port=htons(40011);                  // PORT number ranges from 1024 to 49151                                                                        
        medaddr.sin_addr.s_addr=htonl(INADDR_ANY);                                                                                                                      
                                                                                                                                                                        
        serv1addr.sin_family=AF_INET;                                                                                                                                   
        serv1addr.sin_port=htons(40002);                  // PORT number ranges from 1024 to 49151                                                                      
        serv1addr.sin_addr.s_addr=htonl(INADDR_ANY);                                                                                                                    
                                                                                                                                                                        
        serv2addr.sin_family=AF_INET;                                                                                                                                   
        serv2addr.sin_port=htons(40003);                  // PORT number ranges from 1024 to 49151                                                                      
        serv2addr.sin_addr.s_addr=htonl(INADDR_ANY);                                                                                                                    
                                                                                                                                                                        
        if(bind(sockdesc,(struct sockaddr *)&medaddr,sizeof(medaddr)) < 0)                                                                                              
        {                                                                                                                                                               
                printf("Bind Failed");                                                                                                                                  
                return -1;                                                                                                                                              
        }                                                                                                                                                               
                                                                                                                                                                        
        int len=sizeof(cliaddr);                                                                                                                                        
        printf("Waiting for a request...\n");                                                                                                                           
        char input[30]="";                                                                                                                                              
        strcpy(input," ");                                                                                                                                              
        char n[10];                                                                                                                                                     
        int output[1]={0};                                                                                                                                              
        char space=' ';                                                                                                                                                 
        recvfrom(sockdesc,input,sizeof(input),0,(struct sockaddr *)&cliaddr,&len);                                                                                      
        if (strcmp(input,"ADD")==0)                                                                                                                                     
        {                                                                                                                                                               
                printf("Enter the no.of inputs-");                                                                                                                      
                scanf("%s",&n);                                                                                                                                         
                scanf("%c",&space);                                                                                                                                     
                sendto(sockdesc,n,strlen(n),0,(struct sockaddr *)&serv1addr,sizeof(serv1addr));                                                                         
                recvfrom(sockdesc,output,sizeof(output),0,(struct sockaddr *)&serv1addr,sizeof(serv1addr));                                                             
                printf("%d-ADD OUTPUT",output[0]);                                                                                                                      
        }                                                                                                                                                               
                                                                                                                                                                        
        if (strcmp(input,"SUB")==0)                                                                                                                                     
        {                                                                                                                                                               
                sendto(sockdesc,input,strlen(input),0,(struct sockaddr *)&serv2addr,sizeof(serv2addr));                                                                 
                recvfrom(sockdesc,output,sizeof(output),0,(struct sockaddr *)&serv2addr,sizeof(serv2addr));
                printf("%d-the output is",output[0]);                                                                                                                   
        }                                                                                                                                                               
                                                                                                                                                                        
                                                                                                                                                                        
        sendto(sockdesc,output,sizeof(output),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));                                                                           
                                                                                                                                                                        
        close(sockdesc);                                                                                                                                                
                                                                                                                                                                        
                                                                                                                                                                        
                                                                                                                                                                        
        return 0;                                                                                                                                                       
}                                    