#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
//#include<error.h>
#include<pthread.h>
#include<time.h>
#include<errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
 */
#include <mcrypt.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 10024

static int  server_address_size, client_address_size;
static struct sockaddr_in client, server;

  char* IV = "AAAAAAAAAAAAAAAA";
  char *key = "0123456789abcdef";
  int key_len = 16; /* 128 bits */
  char* buffer;
  int buffer_len = 100;
int encrypt(
    void* buffer)
	{
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}

  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}

int decrypt(
    void* buffer
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}

void display(char* ciphertext, int len){
  int v;
  for (v=0; v<len; v++){
    printf("%d ", ciphertext[v]);
  }
  printf("\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd2, const void*data, size_t size) {
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        if ((r = write(fd2, data, size)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}

int fd,fd1;
void* receive()
{

FILE *file_out ;    
	file_out=fopen("output1.txt","w+"); 
char *name="paserver";
    pa_simple *s = NULL;
    /* The Sample format to use */
static const   pa_sample_spec ss1 = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };     
    int ret = 1;
    int error;
   /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss1, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }  

     uint8_t buf[BUFSIZE];
     uint8_t recvbuf[BUFSIZE];
int len;
ssize_t r;
int file = fileno(file_out);
while(1)
{
   
   len = recvfrom(fd, recvbuf, sizeof(recvbuf),0,(struct sockaddr *)&client, &client_address_size) ;
   if( len <=0)
   {
       perror("receive");
      exit(4);
   }
   
   uint8_t * recvbuffpt=recvbuf;
			
			  decrypt(recvbuffpt);
   
  
        /* And write it to STDOUT */
       if (loop_write(file, recvbuf, sizeof(recvbuf)) != sizeof(recvbuf)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
            }  

			
 if (pa_simple_write(s, recvbuf, sizeof(recvbuf) , &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
      }

 /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
        } 
 

  }
   ret = 0;

finish:

    if (s)
        pa_simple_free(s);

}
void* send1()
{
/* The sample type to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;        
      char *name=" paclient ";
   /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, name , PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }


uint8_t sendbuf[BUFSIZE];
while(1)
{
      
         uint8_t buf[BUFSIZE];

    
   /* Record some data ... */
        if (pa_simple_read(s, sendbuf, sizeof(sendbuf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

   			uint8_t * recvbuffpt=sendbuf;
			
  encrypt(recvbuffpt); 


   
if(sendto(fd,sendbuf,sizeof(sendbuf),0,(struct sockaddr *)&client,sizeof(client))<0)
{
	perror("send");
	exit(5);
}
//pa_simple_free(s); 
}


   ret = 0;

finish:

    if (s)
        pa_simple_free(s);
}








int main( int argc, char*argv[])

{


   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       exit(1);
   }

   /*
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network
    * interface.)
    */
   server.sin_family      = AF_INET;  /* Server is in Internet Domain */
   server.sin_port        = 0;         /* Use any available port      */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */
   
   if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
      
       exit(2);
   }
   /* Find out what port was really assigned and print it */
   server_address_size = sizeof(server);
   if (getsockname(fd, (struct sockaddr *) &server, &server_address_size) < 0)
   {  
       exit(3);
   }
   printf("Port assigned is %d\n", ntohs(server.sin_port));
   /*
    * Receive a message on socket s in buf  of maximum size 32
    * from a client. Because the last two paramters
    * are not null, the name of the client will be placed into the
    * client data structure and the size of the client address will
    * be placed into client_address_size.
    */
 /*  if(listen(fd,1)<0)
   {
   perror("listen");
   exit(4);
   }
   client_address_size = sizeof(client);
   if((fd1=accept(fd, (struct sockaddr *) &client,
            &client_address_size)) <0)
   {
       perror("recvfrom");
      exit(4);
   }
   /*
    * Print the message and the name of the client.
    * The domain should be the internet domain (AF_INET).
    * The port is received in network byte order, so we translate it to
    * host byte order before printing it.
    * The internet address is received as 32 bits in network byte order
    * so we use a utility that converts it to a string printed in
    * dotted decimal format for readability.
    */
  /* printf("Received message   from domain %s port %d internet\
 address %s\n",
       
       (client.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
       ntohs(client.sin_port),

       inet_ntoa(client.sin_addr));
       
client_address_size=sizeof(client);
*/
int buf[10024];
static  int len=0;
len=recvfrom(fd, buf, sizeof(buf),0,(struct sockaddr *)&client, &client_address_size) ;   
pthread_t p1,p2,p3;
pthread_create(&p1,NULL,send1,NULL);
pthread_create(&p2,NULL,receive,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
return 0;

}
