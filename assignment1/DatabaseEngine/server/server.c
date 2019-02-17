// C Program for Message Queue (Reader Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <ctype.h>
#include<string.h> 
#include <stdlib.h> 

// structure for message queue 
struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[30]; 
} message_recv; 

struct mesg_buffer_recv { 
    long mesg_type; 
    char mesg_text; 
} message_send;
  
int main() 
{ 
key_t key[2]; 
int msgid[2]; 
int clientid;
int serverid = 9;
char c;
FILE *fptr; 

    // ftok to generate unique key 
    key[0] = ftok("progfile", 1); 
    key[1] = ftok("from server", 123); 

    // msgget creates a message queue 
    // and returns identifier 
  msgid[0] = msgget(key[0], 0666 | IPC_CREAT); 
  msgid[1] = msgget(key[1], 0666 | IPC_CREAT); 

  message_send.mesg_type = serverid;
 
  
  while(1)
{
  for(clientid=2 ; clientid<7; clientid++)
  {     
	if(msgrcv(msgid[0], &message_recv, sizeof(message_recv), clientid, IPC_NOWAIT)==-1)
	{
	continue;
	perror("msgrcv failed with error\n");
	}

       printf("Data Received from client%d is : %s \n",clientid, message_recv.mesg_text); 

       fptr = fopen(message_recv.mesg_text,"r");  
       if (fptr == NULL) 
       { 
        printf("Cannot open file \n"); 
        exit(0); 
       } 
  
    // Read contents from file 
    c = fgetc(fptr); 

    while (c != EOF) 
    { 
        printf ("%c", c);  
        message_send.mesg_text = c;
        msgsnd(msgid[1], &message_send, sizeof(message_send),1 ); 
        c = fgetc(fptr);
    } 

}
}

  
    // to destroy the message queue 
   msgctl(msgid[0], IPC_RMID, NULL); 
   msgctl(msgid[1], IPC_RMID, NULL); 

    return 0; 
} 
