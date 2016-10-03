#ifndef DATA_SERVER_SOCKET_H
#define	DATA_SERVER_SOCKET_H

#include <netinet/in.h> 
    
                                                                                /*SOCKET...*/
int my_socket(void);                                                            /*...creation*/
void my_bind(int sock ,struct sockaddr * serverptr ,int server_size);           /*...bind socket to address*/
void my_listen(int sock,int connection_limit);                                  /* ...listen for connections*/
int my_accept(int sock,struct sockaddr * clientptr ,int * clientlen);

#endif	/* DATA_SERVER_SOCKET_H */

