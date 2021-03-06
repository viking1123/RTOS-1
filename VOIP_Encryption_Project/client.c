
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include <stdio.h>
#include<string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<error.h>
#include<sys/socket.h>
#include <pthread.h>
#include<errno.h>


//////////////////////////////////
/*
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
 */
#include <mcrypt.h>

#include <math.h>
#include <stdint.h>

//////////////////////////////

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 10024
///////////////////////////////////////////////////





MCRYPT td, td2;
  //char * plaintext = "oû<82>û<82>ûsû<97>û<84>û<9c>ûYûmû<90>";
  char* IV = "AAAAAAAAAAAAAAAA";
  char *key = "0123456789abcdef";
  int keysize = 16; /* 128 bits */
  char* buffer;
  int buffer_len = 10024;




int encrypt(
    void* buffer/* Because the plaintext could include null bytes*/
     
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}

  mcrypt_generic_init(td, key, keysize, IV);
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
  
  mcrypt_generic_init(td, key, keysize, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}
/*
void display(char* ciphertext, int len){
  int v;
  for (v=0; v<len; v++){
    printf("%d ", ciphertext[v]);
  }
  printf("\n");
}
*/

////////////////////////////////////////////////////////////////////////


 static struct sockaddr_in server;
 static int server_address_size;
/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, const void*data, size_t size) {
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        if ((r = write(fd, data, size)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}




static int fd;
void * receive()
{    pa_simple *s = NULL;
FILE *file_out ;    
	file_out=fopen("output1.txt","w+"); 
char *name="paserver";
server_address_size=sizeof(server);
  /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };
    
   

  //  pa_simple *s = NULL;
    int ret = 1;
    int error;
    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
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
 


   len = recvfrom(fd, recvbuf, sizeof(recvbuf),0,(struct sockaddr *)&server, &server_address_size) ;
   if( len <=0)
   {
       perror("receive");
      exit(4);
   }

	uint8_t* recvbufpt=recvbuf;
  decrypt(recvbufpt);
  
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
    
 //pa_simple_free(s);
  }
   ret = 0;

finish:

    if (s)
        pa_simple_free(s);

}


void * send1()
{

 
    pa_simple *s = NULL;
    int ret = 1;
    int error;        
      char *name=" paclient ";
        uint8_t buf[BUFSIZE];
          static const pa_sample_spec sss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };
   
    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, name , PA_STREAM_RECORD, NULL, "record", &sss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }  


uint8_t sendbuf[BUFSIZE];

//uint8_t buffer[BUFSIZE];
while(1)
{
    
   
   /* Record some data ... */
        if (pa_simple_read(s, sendbuf, sizeof(sendbuf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

//buffer=sendBuf;
uint8_t* sendbufpt=sendbuf;
 encrypt(sendbufpt);    
if(sendto(fd,sendbuf,sizeof(sendbuf),0,(struct sockaddr *)&server , sizeof(server))<0)
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

void  main( int argc,  char*argv[])
{
static int server_address_size;
static int n;
int i;
unsigned short port;
   /* argv[1] is internet address of server argv[2] is port of server.
    * Convert the port from ascii to integer and then from host byte
    * order to network byte order.
    */
   if(argc != 3)
   {
      printf("Usage: %s <host address> <port> \n",argv[0]);
      exit(1);
   }
   port = htons(atoi(argv[2]));
   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       perror("socket");
       exit(2);
   }

   /* Set up the server name */
   server.sin_family      = AF_INET;            /* Internet Domain    */
   server.sin_port        = port;               /* Server Port        */
   server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */
/* if(connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf(" non `Connect()");
        exit(3);
    } */






pthread_t p1,p2,p3;
pthread_create(&p1,NULL,send1,NULL);
pthread_create(&p2,NULL,receive,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
}

