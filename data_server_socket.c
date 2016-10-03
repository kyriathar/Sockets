#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>         /* s o c k e t s */
#include <sys/types.h>        /* s o c k e t s */
#include <sys/socket.h>       /* s o c k e t s */
#include <netinet/in.h>       /* i n t e r n e t s o c k e t s */

#include "data_server_my_functions.h"
#include "data_server_socket.h"
                            
int my_socket(void)                             /*creation*/
{
    int sock ;
    if (( sock = socket(PF_INET ,SOCK_STREAM, 0) ) < 0)     /*TCP socket*/
        perror_exit("Server : my_socket") ;
    return sock ;
}

void my_bind(int sock ,struct sockaddr * serverptr ,int server_size)   /*bind*/
{
    if ( bind(sock ,serverptr ,server_size ) < 0 )
        perror_exit("Server : my_bind") ;
}

void my_listen(int sock,int connection_limit)                       /* listen for connections*/
{
    if ( listen(sock ,connection_limit ) < 0 ) 
        perror_exit("Server : my_listen") ;
}

int my_accept(int sock,struct sockaddr * clientptr ,int * clientlen)
{
    int newsock ;
    if (( newsock = accept( sock , clientptr , clientlen) ) < 0 ) 
        perror_exit("Server : my_accept") ;
    return newsock;
}