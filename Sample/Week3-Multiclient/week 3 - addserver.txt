#include<stdio.h>                                                                                                                                                       
#include<sys/types.h>                                                                                                                                                   
#include<netinet/in.h>                                                                                                                                                  
#include<sys/socket.h>                                                                                                                                                  
#include<string.h>                                                                                                                                                      
#include<unistd.h>                                                                                                                                                      
int main()                                                                                                                                                              
{                                                                                                                                                                       
        int sockdesc;                                                                                                                                                   
        struct sockaddr_in serv1addr,medaddr;                                                                                                                           
                                                                                                                                                                        
        sockdesc=socket(AF_INET,SOCK_DGRAM,0);                                                                                                                          
        if(sockdesc==-1)                                                                                                                                                
        {                                                                                                                                                               
                printf("Socket not created");                                                                                                                           
                return -1;                                                                                                                                              
        }                                                                                                                                                               
                                                                                                                                                                        
        serv1addr.sin_family=AF_INET;                                                                                                                                   
        serv1addr.sin_port=htons(40002);                        // PORT number ranges from 1024 to 49151                                                                
        serv1addr.sin_addr.s_addr=htonl(INADDR_ANY);                                                                                                                    
        if(bind(sockdesc,(struct sockaddr *)&serv1addr,sizeof(serv1addr)) < 0)                                                                                          
        {                                                                                                                                                               
                printf("Bind Failed");                                                                                                                                  
                return -1;                                                                                                                                              
        }                                                                                                                                                               
        int len=sizeof(medaddr);                                                                                                                                        
        printf("Waiting for a request...\n");                                                                                                                           
        char input[30];                                                                                                                                                 
        int output[1];                                                                                                                                                  
        strcpy(input," ");                                                                                                                                              
        recvfrom(sockdesc,input,sizeof(input),0,(struct sockaddr *)&medaddr,&len);                                                                                      
        int n=atoi(input);                                                                                                                                              
        int temp;                                                                                                                                                       
        printf("Enter the numbers- ");                                                                                                                                  
        while(n>0)                                                                                                                                                      
        {                                                                                                                                                               
                scanf("%d",&temp);                                                                                                                                      
                output[0]+=temp;                                                                                                                                        
                n--;                                                                                                                                                    
        }                                                                                                                                                               
        printf("The sum is %d",output[0]);                                                                                                                              
        sendto(sockdesc,output,sizeof(output),0,(struct sockaddr *)&medaddr,sizeof(medaddr));                                                                           
        close(sockdesc);                                                                                                                                                
                                                                                                                                                                        
                                                                                                                                                                        
                                                                                                                                                                        
        return 0;                                                                                                                                                       
}                                 