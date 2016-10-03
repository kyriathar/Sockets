#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h> /* PATH_MAX */

#include "remote_client_my_functions.h"
#include "remote_client_socket.h"
#include "remote_client_safe_read_write.h"
#include "remote_client_get_files.h"


int main(int argc, char** argv) {
    
    int c;
    int sock ;
    int port ;
    int folder_size ;
    char hostname[32];
    char * folder ;
    char * path ;
    char * server_ip ;
    
    struct sockaddr_in server ;
    struct sockaddr * serverptr =(struct sockaddr *)&server ;
    struct hostent * rem ;
    
    if(argc != 7)
    {
        printf("Wrong arguments : usage is ./remoteClient -i <server_ip> -p <server_port> -d <directory>\n");
        return (EXIT_SUCCESS);
    }
    
    while ((c = getopt (argc, argv, "i:p:d:")) != -1)
        switch (c)
        {
           case 'i':
                /*server ip*/
                server_ip = optarg ;
                break;
           case 'p':
                /*server port*/
               port = atoi(optarg) ;
                break;
           case 'd':
                /*directory*/
                path = optarg ;
                break;
           default:
               printf("wrong optargument\n");
        }
        
    printf("Client’s parameters are:\n");
    printf("\tserverIP: %s\n\tport: %d\n\tdirectory: %s\n",server_ip,port,path);    
    
    /*Initialize stuff*/
    memset(hostname,'\0',32);
    folder_size = -1 ;
    
    /*socket creation*/
    sock = my_socket();                                                     
    
    /* Find server address */
    rem = my_gethostbyname(server_ip);
        
    server.sin_family = AF_INET ;                               /* Internet domain */
    memcpy (&server.sin_addr , rem->h_addr , rem->h_length) ;
    server.sin_port = htons(port) ;                             /* Server port */
    
    /*Initiate connection*/
    my_connect(sock ,serverptr ,sizeof(server));
    printf ( "Connecting to %s port %d \n" , hostname , port) ;
    
    folder = my_malloc(strlen(path)*sizeof(char) +1);
    memset(folder,'\0',strlen(path)*sizeof(char) +1);
    strcpy(folder,path);
    
    folder_size = strlen(folder)*sizeof(char);
    my_write(sock,&folder_size,sizeof(int));
    my_write(sock,folder,folder_size);
    
    
    
    /*get file contents*/
    if( strcmp(folder," ")!=0 )
        getFiles(sock);
    
    /*close socket*/
    close(sock);
    
    free(folder);
    
    printf("Client exiting...\n");
    return (EXIT_SUCCESS);
}

 



