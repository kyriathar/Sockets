#ifndef REMOTE_CLIENT_SOCKET_H
#define	REMOTE_CLIENT_SOCKET_H

#include <netdb.h>
                                                                                /*SOCKET...*/
int my_socket(void);                                                            /*...creation*/
struct hostent * my_gethostbyname(char * host);                                 /*find server address*/                                 
void my_connect(int sock ,struct sockaddr * serverptr ,int server_size);        /*Initiate connection*/


#endif	/* REMOTE_CLIENT_SOCKET_H */

