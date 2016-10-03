#ifndef DATA_SERVER_LIST_H
#define	DATA_SERVER_LIST_H

typedef struct Job Job ;
typedef struct Node Node ;
typedef struct List List ;

struct Job{
    char * filepath ;
    int sock ;
};

struct Node {
    Job * job ;
    Node * next ;
};

struct List{
    Node * first ;
    Node * current ;
    int size ;
};

void initJob(Job * job);
void initNode(Node * node);
void initList(List * list); 

void addToList(List * list ,char * filename , int sock);

Node * getCurrent(List * list);

void freeList(List * list);

#endif	/* DATA_SERVER_LIST_H */

