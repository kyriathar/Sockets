#ifndef DATA_SERVER_FIFO_ARRAY_H
#define	DATA_SERVER_FIFO_ARRAY_H

#define MAX_SOCKETS 512

typedef struct Cell Cell ;
typedef struct Fifo Fifo ;

struct Cell{
    char * file_name ;
    int sock ;
};

struct Fifo{
    Cell * cell_array ;
    int start ;
    int end ;
    int count ;
};

void initCell(Cell * cell);
void initFifo(Fifo * fifo ,int queue_size);

void setCell(Cell * cell , char * file_name , int sock);

void addToFifo(Fifo * fifo , int queue_size , char * file_name , int sock);
void removeFromFifo(Fifo * fifo , int queue_size , char ** file_name , int * sock);

void freeFifo(Fifo * fifo, int queue_size);

//void printFifo(Fifo * fifo);

#endif	/* DATA_SERVER_FIFO_ARRAY_H */

