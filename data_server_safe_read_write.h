#ifndef SAFE_READ_WRITE_H
#define	SAFE_READ_WRITE_H


int my_open( char * pathname );                                      /*for reading only*/
int my_read(int sock , void * buffer , int X);                      /*safe read*/
int my_write(int sock,void * buffer,int X);                      /*safe write*/

#endif	/* SAFE_READ_WRITE_H */

