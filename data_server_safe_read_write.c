#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "data_server_safe_read_write.h"

int my_open( char * pathname )                                      /*for reading only*/
{
    int fd ;
    
    if( (fd = open(pathname,O_RDONLY)) == -1 )
        perror("my_open");
    
    return fd ;
}


int my_read(int sock , void * buffer , int X)                      /*safe read*/
{
    int n ;
    int bytes_read =0;

    while(bytes_read < X)
    {
        n = read(sock, buffer + bytes_read , X - bytes_read);
        if(n<1)
        {    
            if(n==0)
            {}    //printf("my_read : nothing more to read\n");
            else
                perror("my_read");
            break ;
        }
        bytes_read = bytes_read + n ;    
    }
    return bytes_read;
}

int my_write(int sock,void * buffer,int X)                      /*safe write*/
{
    int n ;
    int bytes_written =0;

    while(bytes_written < X)
    {
        n = write(sock, buffer + bytes_written , X - bytes_written);
        if(n<1)
        {    
            if(n==0)
                printf("my_write : nothing more to write\n");
            else
                perror("my_write");
            break ;
        }
        bytes_written = bytes_written + n ;    
    }
    return bytes_written;
}
