#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_server_my_functions.h"

void * my_malloc(size_t size)
{
    void * ptr ;
    
    ptr = NULL ;
    ptr = malloc(size);
    
    if(ptr == NULL)
    {
        perror("my_malloc");
        exit(1);
    }
    return ptr ;
}

void perror_exit (char * message) 
{
    perror (message) ;
    exit (EXIT_FAILURE) ;
}

void perror_thread(char * message,int error)
{
    fprintf(stderr , "%s : %s \n",message , strerror(error));
}