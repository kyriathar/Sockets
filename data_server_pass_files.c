#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <pthread.h> /* For threads */

#include "data_server_pass_files.h"
#include "data_server_fifo_array.h"
#include "data_server_my_functions.h"


#define FILE_NAME_START         -1
#define FILE_CONTENT_START      -2
#define FILE_CONTENT_END        -3
#define END_OF_ALL              -4
#define FALSE_PATH              -5
#define SOCKET_SAFE_CLOSE       -7

extern Fifo fifo;

extern int remaining_files[MAX_SOCKETS];

extern pthread_mutex_t mtx_array_for_sockets[MAX_SOCKETS];

extern pthread_cond_t cond_full ;

void * FileToSocket( void * ptr )
{
    int fd;
    int bytes_read;
    int removed_sock ;
    int file_name_size;
    /*for option*/
    int option ;
    int queue_size ;
    
    char * removed_str ; 
    char * buffer;
    
    long page_size ;
    
    queue_size = (*(int *)ptr) ;
    
    
    page_size = sysconf(_SC_PAGESIZE);     //page_size = 4096
    buffer =(char *) my_malloc(page_size*sizeof(char));      
    
    while(1)
    {
        /*Init stuff*/
        bytes_read =0;
        removed_sock = -1;
        memset(buffer,'\0',page_size );

        /*getting first filename from list*/
        removeFromFifo(&fifo ,queue_size, &removed_str , &removed_sock);                   //CRITICAL   
        
        printf("[Thread: %ld]: Received task: <%s, %d>\n",pthread_self(),removed_str,removed_sock);
        
        /*from this point => Critical section over writing to socket */
        
        pthread_mutex_lock(&mtx_array_for_sockets[removed_sock]);
        
        //printf("removed_sock = %d \n",removed_sock);
        
        /*server termination->worker termination*/
        if( strcmp(removed_str," ")==0 )
        {
            free(buffer);
            free(removed_str);
            pthread_mutex_unlock(&mtx_array_for_sockets[removed_sock]);
            pthread_exit(NULL) ;
        }
        
        
        /*removed_str has our file*/
        fd = my_open(removed_str);
        /*wrong path given */
        if(fd == -1 )
        {
            option = FALSE_PATH;
            my_write(removed_sock,&option,sizeof(int));                         //CRITICAL
            //fifo.count = 0 ;
        }
        
        /*file is opened here*/

        /*pass filename to socket*/
        option = FILE_NAME_START;
        my_write(removed_sock,&option,sizeof(int));
        
        file_name_size = strlen(removed_str);
        my_write(removed_sock,&file_name_size,sizeof(int));
        my_write(removed_sock,removed_str,file_name_size);

        
        printf("[Thread: %ld]: About to read file %s\n",pthread_self(),removed_str);
        
        free(removed_str);

        /*pass file contents to socket*/
        do{
            bytes_read = my_read(fd,buffer,page_size);
            if( bytes_read ==0 )
                break;
            option = FILE_CONTENT_START;
            my_write(removed_sock,&option,sizeof(int));
            
            my_write(removed_sock,&bytes_read,sizeof(int));                        
            my_write(removed_sock,buffer,bytes_read);                          
            memset(buffer,'\0',page_size );
        }while(bytes_read > 0 );
        
        /*pass end of content*/
        option = FILE_CONTENT_END;
        my_write(removed_sock,&option,sizeof(int));
        close(fd);
    
        remaining_files[removed_sock] -- ;
        /*...close socket */
        if(remaining_files[removed_sock] == 0 )
        {
            my_read(removed_sock,&option,sizeof(int));
            if(option == SOCKET_SAFE_CLOSE)                   //just to make sure client has received all files
            {
                printf("I am closing socket ! \n");
                close(removed_sock);
            }
        }
        
        /*end of critical section*/
        
        pthread_mutex_unlock(&mtx_array_for_sockets[removed_sock]);
    }
}
