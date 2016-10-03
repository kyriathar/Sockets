#ifndef DATA_SERVER_MY_FUNCTIONS_H
#define	DATA_SERVER_MY_FUNCTIONS_H

void * my_malloc(size_t size);
void perror_exit(char * message);
void perror_thread(char * message,int error);

#endif	/* DATA_SERVER_MY_FUNCTIONS_H */

