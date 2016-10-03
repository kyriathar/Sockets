#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "remote_client_get_files.h"
#include "remote_client_my_functions.h"
#include "remote_client_create_dirsFile.h"

#define FILE_NAME_START         -1
#define FILE_CONTENT_START      -2
#define FILE_CONTENT_END        -3
#define END_OF_ALL              -4
#define FALSE_PATH              -5
#define NO_OF_FILES             -6
#define SOCKET_SAFE_CLOSE       -7

void getFiles( int sock )
{
    int fd ;
    int file_count = 0;
    int option ;
    int file_name_size ;
    int no_of_files = -1 ;
    int file_content_size;
    char * file_path ;
    char * file_content;
    
    //printf("sock = %d\n",sock);
    
    do
    {
        /*get option*/
        my_read(sock,&option, sizeof(int));
        
        switch(option)
        {
            case FILE_NAME_START:
                /*get file name*/
                my_read(sock,&file_name_size, sizeof(int));
                file_path =(char *)my_malloc(file_name_size*sizeof(char)+1);          
                memset(file_path,'\0',file_name_size*sizeof(char)+1);
                my_read(sock,file_path, file_name_size);                        //file_path has file name (path)
                
                printf("Received: %s\n",file_path);
                
                /*create path and file and keep file open*/
                fd = createDirsFile(file_path);
                
                free(file_path);
                file_path = NULL ;
                
                break;
            case FILE_CONTENT_START:
                /*fill file*/
                my_read(sock,&file_content_size, sizeof(int));                     
                file_content = (char *)my_malloc(file_content_size*sizeof(char));             
                memset(file_content,'\0',file_content_size*sizeof(char));
                my_read(sock,file_content, file_content_size);                                             
                
                my_write(fd,file_content,file_content_size);
                
                free(file_content);
                file_content = NULL;
                break;
            case FILE_CONTENT_END:
                /*close file*/
                close(fd);
                file_count ++ ;
                break;
            case FALSE_PATH :
                printf("False path so i 'm exiting now !\n");
                break ;
                //exit(1);
            case NO_OF_FILES:
                my_read(sock,&no_of_files, sizeof(int));
                printf("Client : i need to get %d files \n",no_of_files) ;
                break;
            default:
                //printf("getFiles : error at option!\n");
                printf("option = %d\n",option);
                //exit(1);
                break ;
        }
        
    }while(file_count < no_of_files);
    
    option = SOCKET_SAFE_CLOSE ;
    my_write(sock,&option,sizeof(int));
}
