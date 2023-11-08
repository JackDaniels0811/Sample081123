#include<stdio.h>                                                                                                                                                       
#include<sys/types.h>                                                                                                                                                   
#include<netinet/in.h>                                                                                                                                                  
#include<sys/socket.h>                                                                                                                                                  
#include<string.h>                                                                                                                                                      
#include<unistd.h>                                                                                                                                                      
int main()                                                                                                                                                              
{                                                                                                                                                                       
        int sockdesc;                                                                                                                                                   
        struct sockaddr_in serv2addr,medaddr;                                                                                                                           
                                                                                                                                                                        
        sockdesc=socket(AF_INET,SOCK_DGRAM,0);                                                                                                                          
        if(sockdesc==-1)                                                                                                                                                
        {                                                                                                                                                               
                printf("Socket not created");                                                                                                                           
                return -1;                                                                                                                                              
        }                                                                                                                                                               
                                                                                                                                                                        
        serv2addr.sin_family=AF_INET;                                                                                                                                   
        serv2addr.sin_port=htons(40003);                        // PORT number ranges from 1024 to 49151                                                                
        serv2addr.sin_addr.s_addr=htonl(INADDR_ANY);                                                                                                                    
        if(bind(sockdesc,(struct sockaddr *)&serv2addr,sizeof(serv2addr)) < 0)                                                                                          
        {                                                                                                                                                               
                printf("Bind Failed");                                                                                                                                  
                return -1;                                                                                                                                              
        }                                                                                                                                                               
        int len=sizeof(medaddr);                                                                                                                                        
        printf("Waiting for a request...\n");                                                                                                                           
        char input[30];                                                                                                                                                 
        int num1;                                                                                                                                                       
        int num2;                                                                                                                                                       
        int output[1];                                                                                                                                                  
        strcpy(input," ");                                                                                                                                              
        printf("Enter 1st number- ");                                                                                                                                   
        scanf("%d",&num1);                                                                                                                                              
        printf("Enter 2nd number- ");                                                                                                                                   
        scanf("%d",&num2);                                                                                                                                              
        recvfrom(sockdesc,input,sizeof(input),0,(struct sockaddr *)&medaddr,&len);                                                                                      
        output[0]=num2-num1;                                                                                                                                            
        printf("Th difference is %d",output[0]);                                                                                                                        
        sendto(sockdesc,output,1,0,(struct sockaddr *)&medaddr,sizeof(medaddr));                                                                                        
        close(sockdesc);                                                                                                                                                
                                                                                                                                                                        
                                                                                                                                                                        
                                                                                                                                                                        
        return 0;                                                                                                                                                       
}                                                                                                                                                                       
                                