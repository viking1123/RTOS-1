//Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_KEY_PATHNAME "/tmp"
#define PROJECT_ID 'M'

struct message_text {
    int qid;
    char buf ;
};

struct message {
    long message_type;
    struct message_text message_text;
};

void delay(int number_of_seconds);

int main (int argc, char **argv)
{
    key_t server_queue_key;
    int server_qid, myqid;
    char ch;
    clock_t start, end;
    double total_elapsed_time;

    struct message my_message, return_message;

    // create my client queue for receiving messages from server
    if ((myqid = msgget (IPC_PRIVATE, 0660)) == -1) {
        perror ("msgget: myqid");
        exit (1);
    }

server_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);

    if (server_queue_key== -1) {
        perror ("ftok");
        exit (1);
    }

    if ((server_qid = msgget (server_queue_key, 0)) == -1) {
        perror ("msgget: server_qid");
        exit (1);
    }

    my_message.message_type = 1;
    my_message.message_text.qid = myqid;

    printf ("Please type a message: ");
    ch = getchar();
 do
{
       my_message.message_text.buf = ch;
        // send message to server
 start = clock(); 
        if (msgsnd (server_qid, &my_message, sizeof (struct message_text), 0) == -1) {
            perror ("client: msgsnd");
            exit (1);
        }

        // read response from server
        if (msgrcv (myqid, &return_message, sizeof (struct message_text), 0, 0) == -1) {
            perror ("client: msgrcv");
            exit (1);
        }
    end = clock(); 
total_elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        // process return message from server
        printf ("Message received from server: %c in time %lf\n", return_message.message_text.buf,total_elapsed_time);  

        printf ("Please type a message: Enter 0 to exit ");
        scanf("%c",&ch);
 }while (ch!= '0');

    // remove message queue
    if (msgctl (myqid, IPC_RMID, NULL) == -1) {
            perror ("client: msgctl");
            exit (1);
    }

    printf ("Client: bye\n");

    exit (0);
}

