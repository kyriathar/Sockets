#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>        /* s o c k e t s */
#include <sys/socket.h>       /* s o c k e t s */
#include <netinet/in.h>       /* i n t e r n e t s o c k e t s */

#include "remote_client_socket.h"
#include "remote_client_my_functions.h"


int my_socket(void)                             /*creation*/
{
    int sock ;
    if (( sock = socket(PF_INET ,SOCK_STREAM, 0) ) < 0)     /*TCP socket*/
        perror_exit("Client : my_socket") ;
    return sock ;
}

struct hostent * my_gethostbyname(char * host)                                 /*find server address*/
{
    struct hostent * rem ;
    if (( rem = gethostbyname(host)) == NULL ) 
    {
        herror ( "Client : gethostbyname" ) ; 
        exit (1) ;
    }
    return rem ;
}

void my_connect(int sock ,struct sockaddr * serverptr ,int server_size)        /*Initiate connection*/
{
    if ( connect (sock ,serverptr ,server_size) < 0)
        perror_exit("Client : connect");
}