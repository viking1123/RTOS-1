
// C Program for Message Queue (Writer Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

  
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
    int clientid = 2;
    int serverid = 9;
  
    // ftok to generate unique key 
    key[0] = ftok("progfile", 1); 
    key[1] = ftok("from server", 12);
  
    // msgget creates a message queue 
    // and returns identifier 

  msgid[0] = msgget(key[0], 0666 | IPC_CREAT); 
  msgid[1] = msgget(key[1], 0666 | IPC_CREAT);
     message[0].mesg_type = clientid; 
    message[1].mesg_type = serverid; 
  
    printf("Write Data : "); 
    message[0].mesg_text = getchar(); 
  
    // msgsnd to send message 
    msgsnd(msgid[0], &message[0], sizeof(message[0]), 1); 
  
    // display the message 
    printf("Data send is : %c \n", message[0].mesg_text); 


 msgrcv(msgid[1], &message[1], sizeof(message[1]), serverid, 0); 
  

    // display the message 
    printf("Data received is : %c \n", message[1].mesg_text); 
    return 0; 
} 
