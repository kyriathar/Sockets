#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "data_server_fifo_array.h"
#include "data_server_my_functions.h"

extern pthread_mutex_t mtx_fifo ;
extern pthread_cond_t cond_empty ;
extern pthread_cond_t cond_full ;

void initCell(Cell * cell)
{
    if(cell == NULL)
        printf("initCell : cell == NULL\n");
    else
    {
        cell->file_name =NULL;
        cell->sock = -1 ;                   /*if sock ==-1  => Cell not used*/
    }
}

void initFifo(Fifo * fifo,int queue_size)
{
    int i ;
    
    if(fifo == NULL)
        printf("initFifo : fifo == NULL\n");
    else
    {
        fifo->cell_array = (Cell *)my_malloc(queue_size*sizeof(Cell));
        for(i=0;i<queue_size;i++)
        {
            initCell(&(fifo->cell_array[i]));
        }
        fifo->start = 0;
        fifo->end = -1 ;
        fifo->count = 0 ;
    }
}

void setCell(Cell * cell , char * file_name , int sock)
{
    if(cell == NULL || file_name == NULL)
        printf("setCell : cell == NULL or file_name == NULL\n");
    else
    {                   
        cell->file_name =(char *)my_malloc(strlen(file_name)*sizeof(char)+1);                 //FREE
        memset(cell->file_name,'\0',strlen(file_name)*sizeof(char)+1);
        strcpy(cell->file_name,file_name);
        cell->sock = sock ;
    }
}

void addToFifo(Fifo * fifo , int queue_size , char * file_name , int sock)                       //++++ SINXRONISMO GIA NIMATA
{
    pthread_mutex_lock(&mtx_fifo);
    if(fifo == NULL || file_name == NULL)
        printf("addToFifo : fifo == NULL or file_name == NULL\n");
    else
    {   
        while(fifo->count >= queue_size)
        {
            //printf("fifo is full ,will wait\n");
            pthread_cond_wait(&cond_full,&mtx_fifo);
        }
        fifo->end = (fifo->end + 1) % queue_size ;                         
        setCell( &(fifo->cell_array[fifo->end]) , file_name , sock);
        fifo->count ++ ;
        
    }
    pthread_mutex_unlock(&mtx_fifo);
    pthread_cond_broadcast(&cond_empty);
}

void removeFromFifo(Fifo * fifo , int queue_size , char ** file_name , int * sock)               //++++ SINXRONISMO GIA NIMATA
{
    pthread_mutex_lock(&mtx_fifo);
    if(fifo == NULL )
        printf("removeFromFifo : fifo == NULL\n");
    else
    {
        while(fifo->count <= 0)
        {
            //printf("fifo is empty ,will wait\n");
            pthread_cond_wait(&cond_empty,&mtx_fifo);
        }
        *file_name = my_malloc( strlen((fifo->cell_array[fifo->start]).file_name)*sizeof(char) +1 );        //FREE OUTSIDE...
        memset( *file_name , '\0' , strlen((fifo->cell_array[fifo->start]).file_name)*sizeof(char) +1 );
        strcpy( *file_name , (fifo->cell_array[fifo->start]).file_name );
        *sock = (fifo->cell_array[fifo->start]).sock;
        
        /*freeCell*/
        free((fifo->cell_array[fifo->start]).file_name);
        (fifo->cell_array[fifo->start]).file_name = NULL ;
        
        fifo->start = (fifo->start + 1) % queue_size ;
        fifo->count--;
        
    }
    pthread_mutex_unlock(&mtx_fifo);
    pthread_cond_broadcast(&cond_full);
}

void freeFifo(Fifo * fifo , int queue_size)
{
    int i ;
    if(fifo == NULL)
        printf("freeFifo : fifo== NULL \n");
    else
    {
        for(i=0;i< queue_size; i++)
        {
            if(fifo->cell_array[i].file_name != NULL)
                free(fifo->cell_array[i].file_name);
        }
        free(fifo->cell_array);
    }
    return ;
}


/*void printFifo(Fifo * fifo)                                                     //++++ SINXRONISMO GIA NIMATA
{
    int i ;
    pthread_mutex_lock(&mtx_fifo);
    if(fifo == NULL )
        printf("printFifo : fifo == NULL\n");
    else
    {
        printf("FIFO IS : \n");
        for(i=0;i<MATRIX_SIZE;i++)
        {
            printf("position %d ,%s ",i,fifo->cell_array[i].file_name);
            printf("socket number is %d\n",fifo->cell_array[i].sock);
        }
        printf("END OF FIFO  \n");
    }
    pthread_mutex_unlock(&mtx_fifo);
}
 */ 
