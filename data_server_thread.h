#ifndef DATA_SERVER_THREAD_H
#define	DATA_SERVER_THREAD_H

void my_pthread_create(pthread_t * thread, 
                       const pthread_attr_t * attr,
                       void * (*start_routine)(void *), 
                       void *arg);



#endif	/* DATA_SERVER_THREAD_H */

