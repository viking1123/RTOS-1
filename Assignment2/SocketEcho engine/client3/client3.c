#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
#include <time.h> 
  
#define PORT 4486 
//#define BUF_SIZE 2000 
#define BUF_SIZE 2
 
int main(int argc, char**argv) {  
 struct sockaddr_in addr, cl_addr;  
 int sockfd, ret;  
 char buffer[BUF_SIZE];  
//char buffer;
 struct hostent * server;
 char * serverAddr;
clock_t start, end;
double total_elapsed_time;


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

 //memset(buffer, 0, BUF_SIZE);
 printf("Enter your message(s): ");

// while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
   printf("enter character");
   buffer[0] = getchar();
do
{
 start = clock(); 
  ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr)); 
  //ret=send(sockfd, buffer, sizeof(buffer), 0);  
  if (ret < 0) {  
   printf("Error sending data!\n\t-%s", buffer);  
  }
  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
   end = clock(); 
  if (ret < 0) {  
   printf("Error receiving data!\n");    
  } else {
   printf("Received: ");
   fputs(buffer, stdout);
total_elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
 // process return message from server
printf ("Time taken to receive message is %lf\n",total_elapsed_time); 
   printf("\n");
  } 
  printf ("Please type a message: Enter 0 to exit ");
  scanf("%c",&buffer[0]);
 }while (buffer[0]!= '0');
 return 0;    
} 
