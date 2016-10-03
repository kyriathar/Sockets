#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>         /* s o c k e t s */
#include <sys/types.h>        /* s o c k e t s */
#include <sys/socket.h>       /* s o c k e t s */
#include <netinet/in.h>       /* i n t e r n e t s o c k e t s */
#include <pthread.h>
#include <unistd.h>
#include <pthread.h> /* For threads */


#include "data_server_enlist_filenames.h"
#include "data_server_my_functions.h"
#include "data_server_socket.h"
#include "data_server_safe_read_write.h"
#include "data_server_fifo_array.h"
#include "data_server_pass_files.h"

Fifo fifo ;

int remaining_files[MAX_SOCKETS];

pthread_mutex_t mtx_array_for_sockets[MAX_SOCKETS];

pthread_mutex_t mtx_fifo ;
pthread_cond_t cond_empty ;
pthread_cond_t cond_full ;


pthread_t producer_tid ;
pthread_t * pool;

int consumer_working ;

int main(int argc, char** argv) {

    int i ;
    int c;
    int sock ;
    int newsock;
    int port ;
    int pool_size ;
    int queue_size ;
    int folder_size ;
    int join_error ;
    char * folder ;
    FirstJob * firstjob ;
    
    socklen_t clientlen ;
    
    struct sockaddr_in server ;
    struct sockaddr_in client ;
    struct sockaddr * serverptr =(struct sockaddr *)&server ;
    struct sockaddr * clientptr =(struct sockaddr *)&client ;
    
    if(argc != 7)
    {
        printf("Wrong arguments : usage is ./dataServer -p <port> -s <thread_pool_size> -q <queue_size>\n");
        return (EXIT_SUCCESS);
    }
    
    while ((c = getopt (argc, argv, "p:s:q:")) != -1)
        switch (c)
        {
           case 'p':
                /*server port*/
                port = atoi(optarg) ;
                break;
           case 's':
                /*thread pool size*/
                pool_size = atoi(optarg);
                break;
           case 'q':
                /*queue size*/
                queue_size = atoi(optarg);
                break;
           default:
               printf("wrong optargument\n");
        }
    
    printf("Server’s parameters are:\n");
    printf("\tport: %d\n\tthread_pool_size: %d\n\tqueue_size: %d\n",port,pool_size,queue_size);
        
    /*Init stuff*/
    consumer_working = 0;
    folder_size = -1;
    firstjob = NULL ;
    initFifo(&fifo,queue_size);
    pthread_mutex_init(&mtx_fifo , 0) ;
    pthread_cond_init(&cond_empty , 0) ;
    pthread_cond_init(&cond_full , 0) ;
    
    for(i=0;i < MAX_SOCKETS ; i++)
        remaining_files[i]=0;
    
    for(i=0;i < MAX_SOCKETS ; i++)
        pthread_mutex_init(&mtx_array_for_sockets[i] , 0) ;
    
    pool =(pthread_t * ) my_malloc(pool_size*sizeof(pthread_t));
    for(i=0 ; i<pool_size ; i++)
    {
        my_pthread_create(&pool[i],NULL,FileToSocket,&queue_size);
    }
    
    printf("Server was successfully initialized...\n");
    
    
    /*socket creation*/
    sock = my_socket();                                                     //CLOSE sock
    
    /*Bind socket to address*/
    server.sin_family = AF_INET;        /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    my_bind(sock ,serverptr ,sizeof(server) );
    
    
    /* listen for connections*/
    my_listen(sock,5);
    
    printf ( "Listening for connections to port %d \n" , port ) ;
    
    /*accept connections*/
    while(1)
    {
        clientlen = sizeof(*clientptr);                             
        newsock = my_accept(sock, /*NULL*/clientptr, /*NULL*/&clientlen);                     
        
        /*reading dirname from socket*/
        my_read(newsock , &folder_size ,sizeof(int));       
        folder = (char * )my_malloc(folder_size +1);
        memset(folder,'\0',folder_size +1);
        my_read(newsock , folder ,folder_size);
        /*create FirstJob*/    
        firstjob =(FirstJob *) my_malloc(sizeof(FirstJob));                     //FREEED
        setFirstJob(firstjob,folder,newsock,queue_size , pool_size);
        /*"producer" thread one per client*/
        my_pthread_create(&producer_tid,NULL,enlistSubFilesWrapper,firstjob);
        
        /*server termination*/
        if(strcmp(folder," ")==0)
        {
            close(sock);
            close(newsock);
            /*join threads*/
            /*join consumer*/
            if(join_error = pthread_join(producer_tid,NULL))
            {
                perror_thread("pthread_join @ consumer",join_error);
                exit(1);
            }
            /*join workers*/
            for(i=0;i<pool_size;i++)
            {
                if(join_error = pthread_join(pool[i],NULL))
                {
                    perror_thread("pthread_join @ workers",join_error);
                    exit(1);
                }
            }
            
            /*destroy condition variables*/
            pthread_cond_destroy(&cond_empty) ;
            pthread_cond_destroy(&cond_full) ;
            
            /*destroy mutexes */
            for(i=0;i < MAX_SOCKETS ; i++)
                pthread_mutex_destroy(&mtx_array_for_sockets[i]) ;
            
            pthread_mutex_destroy(&mtx_fifo) ;
            
            
            free(folder);
            freeFifo(&fifo,queue_size);
            free(pool);
            
            printf("Server exiting...\n");
            
            /*pthread exit*/
            pthread_exit(NULL) ;
        }
        
        free(folder);
        printf("END OF WHILE\n");
    }
    
    return (EXIT_SUCCESS);
}

