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
char ch;
int running=1;
 int clientid;
int serverid = 9;
    // ftok to generate unique key 
    key[0] = ftok("progfile", 1); 
    key[1] = ftok("from server", 12); 

    // msgget creates a message queue 
    // and returns identifier 
  msgid[0] = msgget(key[0], 0666 | IPC_CREAT); 
  msgid[1] = msgget(key[1], 0666 | IPC_CREAT); 
  message[1].mesg_type = serverid;
 
  
  while(1)
{
for(clientid=2 ; clientid<7; clientid++)
{
if(msgrcv(msgid[0], &message[0], sizeof(message[0]), clientid, IPC_NOWAIT)==-1)
{
continue;
perror("msgrcv failed with error\n");
}
    //printf("%d", clientid);
    //printf("%d", msgid[0].msg_lspid);
    printf("Data Received from client%d is : %c \n",clientid, message[0].mesg_text); 
   ch =  message[0].mesg_text;
   printf("%c", ch);
  if(ch>=65 && ch<=90)      //upper case 
  message[1].mesg_text = tolower(ch);
    else if(ch>=97 && ch<=122)//lower case
  message[1].mesg_text = toupper(ch); 
else 
    message[1].mesg_text = message[0].mesg_text;


  printf("%c", message[1].mesg_text);
 msgsnd(msgid[1], &message[1], sizeof(message[1]),1 ); 
}
}

  
    // to destroy the message queue 
   msgctl(msgid[0], IPC_RMID, NULL); 
   msgctl(msgid[1], IPC_RMID, NULL); 

    return 0; 
} 
