#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> /* For threads */

#include "data_server_thread.h"
#include "data_server_my_functions.h"

void my_pthread_create(pthread_t * thread, const pthread_attr_t * attr, void * (*start_routine)(void *), void *arg)
{
    int error ;
    
    if ( error = pthread_create (thread, attr , start_routine , arg ) )   /* New thread */
    { 
        perror_thread(" pthread_create " , error);
        exit (1) ;
    }
}
