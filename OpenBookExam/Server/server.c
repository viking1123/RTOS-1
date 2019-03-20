#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include <ctype.h>
#include"netinet/in.h"

#define PORT 4474
#define BUF_SIZE 20

#define CLADDR_LEN 100

void main() {
 struct sockaddr_in addr, cl_addr;
 int sockfd, len, ret, newsockfd;
char buffer[BUF_SIZE],ch;
char filename[50];
//char buffer,ch;
FILE *fptr1,*fptr2,*fptr3,*fptr4,*fptr5;
int length;
char c;
 pid_t childpid;
 char clientAddr[CLADDR_LEN];
 
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf("Error creating socket!\n");
  exit(1);
 }
 printf("Socket created...\n");
 
 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = INADDR_ANY;
 addr.sin_port = PORT;
 
 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
 if (ret < 0) {
  printf("Error binding!\n");
  exit(1);
 }
 printf("Binding done...\n");

 printf("Waiting for a connection...\n");
 listen(sockfd, 5);

 for (;;) { //infinite loop
  len = sizeof(cl_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
  if (newsockfd < 0) {
   printf("Error accepting connection!\n");
   exit(1);
  }
  printf("Connection accepted...\n");

  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);

   while(1)
{
    fptr1 = 0;
    fptr1 = fopen("sensor1.txt","r"); 
 if (fptr1 == NULL) 
       { 
        printf("Cannot open file1 \n"); 
        exit(0); 
       }  
 fptr2 = 0;
    fptr2 = fopen("sensor2.txt","r");  
 if (fptr2 == NULL) 
       { 
        printf("Cannot open file2 \n"); 
        exit(0); 
       } 
 fptr3 = 0;
    fptr3 = fopen("sensor3.txt","r");  

 if (fptr3 == NULL) 
       { 
        printf("Cannot open file3 \n"); 
        exit(0); 
       } 
 fptr4 = 0;
    fptr4 = fopen("sensor4.txt","r");  
 if (fptr4 == NULL) 
       { 
        printf("Cannot open file4 \n"); 
        exit(0); 
       }
fptr5 = 0; 
    fptr5 = fopen("sensor5.txt","r");  
 if (fptr5 == NULL) 
       { 
        printf("Cannot open file5 \n"); 
        exit(0); 
       } 

  c = fgetc(fptr1); 
      int i=0;
      while (c != EOF) 
       { 
         buffer[i] = c;
         i++;
         c = fgetc(fptr1);
       } 
         buffer[i] = ',';
         i++;
  c = fgetc(fptr2); 
      //int i=0;
      while (c != EOF) 
       { 
         buffer[i] = c;
         i++;
         c = fgetc(fptr2);
       } 
         buffer[i] = ',';
         i++;
  c = fgetc(fptr3); 
      //int i=0;
      while (c != EOF) 
       { 
         buffer[i] = c;
         i++;
         c = fgetc(fptr3);
       }  
         buffer[i] = ',';
         i++; 
c = fgetc(fptr4); 
   //   int i=0;
      while (c != EOF) 
       { 
         buffer[i] = c;
         i++;
         c = fgetc(fptr4);
       }  
         buffer[i] = ',';
         i++;
 c = fgetc(fptr5); 
     // int i=0;
      while (c != EOF) 
       { 
         buffer[i] = c;
         i++;
         c = fgetc(fptr5);
       } 
        

ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);   
    if (ret < 0) {  
     printf("Error sending data!\n");  
     exit(1); 
//buffer = NULL;
    }

    printf("Sent data to %s: %s\n", clientAddr, buffer);
sleep(2);
}
  close(newsockfd);
}
}
