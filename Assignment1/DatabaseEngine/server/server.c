//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_KEY_PATHNAME "/tmp"
#define PROJECT_ID 'M'
#define QUEUE_PERMISSIONS 0660

struct message_text {
    int qid;
    char buf [200];
};

struct message {
    long message_type;
    struct message_text message_text;
};

int main (int argc, char **argv)
{
    key_t msg_queue_key;
    int qid;
    struct message message;
    FILE *fptr; 
    char c;

msg_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);

    if (msg_queue_key == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
        perror ("msgget");
        exit (1);
    }

    printf ("Server: Hello, World!\n");

    while (1) {
        // read an incoming message
        if (msgrcv (qid, &message, sizeof (struct message_text), 0, 0) == -1) {
            perror ("msgrcv");
            exit (1);
        }

        printf ("Server: message received.\n");


       fptr = fopen(message.message_text.buf,"r");  
       if (fptr == NULL) 
       { 
        printf("Cannot open file \n"); 
        exit(0); 
       } 
  
    // Read contents from file 
      c = fgetc(fptr); 
      int i=0;
      while (c != EOF) 
       { 
         message.message_text.buf[i] = c;
         i++;
         c = fgetc(fptr);
       }    
    
        int client_qid = message.message_text.qid;
        message.message_text.qid = qid;

        // send reply message to client
        if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1) {  
            perror ("msgget");
            exit (1);

        }

        printf ("Server: response sent to client.\n");
    }
}
