#ifndef DATA_SERVER_DIR_H
#define	DATA_SERVER_DIR_H

typedef struct FirstJob FirstJob ;

struct FirstJob {
    char * file_name ;
    int sock ; 
    int queue_size ;
    int pool_size ;
};

void setFirstJob(FirstJob * firstjob , char * file_name  ,int sock ,int queue_size ,int pool_size);

void freeFirstJob(FirstJob * firstjob);


void * enlistSubFilesWrapper(void * firstjob); 

#endif	/* DATA_SERVER_DIR_H */

