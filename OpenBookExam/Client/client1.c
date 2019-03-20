#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
#include <time.h> 

  
#define PORT 4474 
#define BUF_SIZE 22

 
int main(int argc, char**argv) {  
 struct sockaddr_in addr, cl_addr;  
 int sockfd, ret;  
 char buffer[BUF_SIZE];
//char buffer;
 struct hostent * server;
 char * serverAddr;
clock_t start, end;
double total_elapsed_time;
time_t t;   // not a primitive datatype
FILE *fptr;

 if (argc < 2) {
  printf("usage: client < ip address >\n");
  exit(1);  
 }

 serverAddr = argv[1]; 
 
 sockfd = socket(AF_INET, SOCK_STREAM, 0);  
 if (sockfd < 0) {  
  printf("Error creating socket!\n");  
  exit(1);  
 }  
 printf("Socket created...\n");   

 memset(&addr, 0, sizeof(addr));  
 addr.sin_family = AF_INET;  
 addr.sin_addr.s_addr = inet_addr(serverAddr);
 addr.sin_port = PORT;     

 ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
 if (ret < 0) {  
  printf("Error connecting to the server!\n");  
  exit(1);  
 }  
 printf("Connected to the server...\n");  

 memset(buffer, 0, BUF_SIZE);
 printf("receive data: ");

fptr=fopen("data.csv","w+");
 if (fptr == NULL) 
       { 
        printf("Cannot open file \n"); 
        exit(0); 
}
fprintf(fptr,"sensor1, sensor2, sensor3, sensor4,sensor5,time\n");

int i=0;
for(i=0;i<25;i++)
{
  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
time(&t);
printf("\nThis program has been writeen at (date and time): %s", ctime(&t)); 

fprintf(fptr,"%s",buffer);
fprintf(fptr,",");
fprintf(fptr,"%s",ctime(&t));


if (ret < 0) {  
   printf("Error receiving data!\n");    
  } else {
   printf("Received: ");
   fputs(buffer, stdout);
   printf("\n");
}

/*int x;
printf("enter 0 to exit");
scanf("%d",&x);
if(x==0)
break;
*/
}

fclose(fptr);
 return 0;    
} 
