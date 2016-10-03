#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <pthread.h> /* For threads */

#include "data_server_enlist_filenames.h"
#include "data_server_fifo_array.h"
#include "data_server_list.h"
#include "data_server_my_functions.h"

#define NO_OF_FILES              -6

extern Fifo fifo;

extern int remaining_files[MAX_SOCKETS];

extern pthread_cond_t cond_empty ;

extern int consumer_working ;

void setFirstJob(FirstJob * firstjob , char * file_name  ,int sock ,int queue_size ,int pool_size)
{
    if(firstjob == NULL || file_name == NULL)
        printf("setJob : firstjob ==NULL or file_name ==NULL\n");
    else
    {
        firstjob->file_name =(char *) my_malloc( strlen(file_name)*sizeof(char) + 1 );
        memset(firstjob->file_name,'\0',strlen(file_name)*sizeof(char) + 1);
        strcpy(firstjob->file_name,file_name);
        firstjob->sock = sock ;
        firstjob->queue_size = queue_size ;
        firstjob->pool_size = pool_size ;
    }
    return ;
}

void freeFirstJob(FirstJob * firstjob)
{
    if(firstjob == NULL )
        printf("freeJob : job ==NULL \n");
    else
    {
        free(firstjob->file_name);
        free(firstjob);
    }
}

void enlistSubFiles( char dirname[] , int sock , List * list)        //THREAD SAFE 
{
    int i ;
    char buffer[1024];    //statiko na ginei dunamika ?
    DIR * dir_ptr ;
    
    //struct dirent * direntp ;
    
    struct dirent entry; 
    struct dirent *result = NULL;
    
    memset(buffer,'\0',1024);
    
    if ( (dir_ptr = opendir(dirname)) == NULL )
    {
        /*whatever that cannot open is a file*/
        /*add to list*/
        addToList(list ,dirname,sock);
    }
    else 
    {
        //while ( (direntp = readdir(dir_ptr)) != NULL )                     // NA ALLAKSEI SE readdir_r gia NIMATA 
        
        while ((readdir_r(dir_ptr,&entry,&result)==0) && result != NULL)
        {
            if( (strcmp(entry.d_name , ".") == 0 ) || (strcmp(entry.d_name , "..") == 0 ) )
            {}
            else
            {
                strcpy(buffer,dirname);
                strcat(buffer,"/");
                strcat(buffer,entry.d_name);
                if(strcmp(entry.d_name,"dataServer")==0)
                    continue;
                
                enlistSubFiles(buffer, sock , list);
                memset(buffer,'\0',1024);
            }
        }
        closedir( dir_ptr ) ;
    }
}


void * enlistSubFilesWrapper(void * firstjob/*char dirname[] , int sock*/)
{
    int i;
    int option ;
    int number_of_files = -1;
    Node * temp = NULL ;
    List list ;
    
    FirstJob * firstjob_ptr = firstjob;
    
    /*server termination->consumer termination*/
    if(strcmp(firstjob_ptr->file_name," ")==0)
    {
        while(fifo.count != 0 || consumer_working > 0)
        {}
            
        for(i=0;i<firstjob_ptr->pool_size;i++)     //change to POOL size
        {
            addToFifo(&fifo ,firstjob_ptr->queue_size, firstjob_ptr->file_name , firstjob_ptr->sock);
        }
        freeFirstJob(firstjob_ptr);
        pthread_exit(NULL) ;
    }
    consumer_working ++;
    initList(&list);
    printf("[Thread: %ld]: About to scan directory %s\n",pthread_self(),firstjob_ptr->file_name); 
    enlistSubFiles(firstjob_ptr->file_name , firstjob_ptr->sock , &list);
    /*list is filled*/
    /*no of files to send*/
    number_of_files = list.size ;   
    /*set remaining files in the array*/
    remaining_files[firstjob_ptr->sock] = number_of_files;
    
    printf("Server : number of files to send= %d\n",number_of_files);
    /*send to server how many files he will receive*/
    option = NO_OF_FILES ;
    my_write(firstjob_ptr->sock,&option,sizeof(int));
    my_write(firstjob_ptr->sock,&number_of_files,sizeof(int));
    
    /*pass filenames from list to fifo*/
    temp = getCurrent(&list);
    while(temp!= NULL)
    {
        printf("[Thread: %ld]: Adding file %s to the queue...\n",pthread_self(),temp->job->filepath);
        addToFifo(&fifo ,firstjob_ptr->queue_size, temp->job->filepath , temp->job->sock);
        temp = getCurrent(&list);
    }
    freeList(&list);
    freeFirstJob(firstjob_ptr);
    consumer_working --;
    pthread_exit(NULL) ;
}
