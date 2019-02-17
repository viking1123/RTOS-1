
// C Program for Message Queue (Writer Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

  
// structure for message queue 
struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[30]; 
    //char mesg_text; 
} message_send; 

struct mesg_buffer_recv { 
    long mesg_type; 
    char mesg_text; 
    //char mesg_text; 
} message_recv;  
  
int main() 
{ 
    //0 - send, 1- receive
    key_t key[2]; 
    int msgid[2]; 
int running=1;
    int clientid = 5;
    int serverid = 9;
  
    // ftok to generate unique key 
    key[0] = ftok("progfile", 1); 
    key[1] = ftok("from server", 123);
  
    // msgget creates a message queue 
    // and returns identifier 

  msgid[0] = msgget(key[0], 0666 | IPC_CREAT); 
  msgid[1] = msgget(key[1], 0666 | IPC_CREAT);
     message_send.mesg_type = clientid; 
    message_recv.mesg_type = serverid; 
  
    printf("Write Data : "); 
    gets(message_send.mesg_text);

    // msgsnd to send message 
    msgsnd(msgid[0], &message_send, sizeof(message_send), 1); 
  
    // display the message 
    printf("Data send is : %s \n", message_send.mesg_text); 



  while(running)
   {
    if(msgrcv(msgid[1], &message_recv, sizeof(message_recv), serverid, 0)==-1)
     {
       perror("msgrcv failed with error\n");
       running=0;
     }
    printf("%c",message_recv.mesg_text);
}


    // display the message 
    printf("Data received is : %c \n", message_recv.mesg_text); 
    return 0; 


} 
