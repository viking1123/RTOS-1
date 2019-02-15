// C Program for Message Queue (Reader Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <ctype.h>
#include<string.h>  

// structure for message queue 
struct mesg_buffer { 
    long mesg_type; 
    //char mesg_text[100]; 
    char mesg_text; 
} message[2]; 
  
int main() 
{ 
    key_t key[2]; 
    int msgid[2]; 
int running=1;
  
    // ftok to generate unique key 
    key[0] = ftok("progfile", 1); 
    key[1] = ftok("from server", 12); 

    // msgget creates a message queue 
    // and returns identifier 
  msgid[0] = msgget(key[0], 0666 | IPC_CREAT); 
  msgid[1] = msgget(key[1], 0666 | IPC_CREAT); 
 
  
  while(running)
{
if(msgrcv(msgid[0], &message[0], sizeof(message[0]), 1, 0)==-1)
{
perror("msgrcv failed with error\n");
running=0;
}
    printf("Data Received is : %c \n",message[0].mesg_text); 
  message[1].mesg_text = toupper(message[0].mesg_text);
    msgsnd(msgid[1], &message[1], sizeof(message[1]), 0); 
}

  
    // to destroy the message queue 
   msgctl(msgid[0], IPC_RMID, NULL); 
   msgctl(msgid[1], IPC_RMID, NULL); 

    return 0; 
} 
