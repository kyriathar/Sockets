#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include "remote_client_create_dirsFile.h"
#include "remote_client_my_functions.h"

int  getArraySize(char * file_name)
{
    int array_size =0;
    char * file_name_temp ;
    char * str;
    
    if(file_name == NULL)
    {
        printf("getArraySize : file_name is NULL\n");
        return -1 ;
    }  
    
    /*must do this for strtok "damages" initiate file name*/
    file_name_temp = (char *) my_malloc(strlen(file_name)*sizeof(char)+1);
    memset(file_name_temp,'\0',strlen(file_name)*sizeof(char)+1);
    strcpy(file_name_temp , file_name);
    
    str = strtok (file_name_temp,"/");
    while (str != NULL)
    {
      array_size++;             /*get array size*/
      str = strtok (NULL, "/");
    }
    free(file_name_temp);
    return array_size ;
}

int createDirsFile(char * file_name )      //Prosoxh na pernaei dynamically allocated string h [] alliws segmentation
{
    int fd ;
    int i;
    int array_size;
    char * file_name_temp ;
    char * str;
    char cur_str[1024] ;             //PROSOXH
    char ** str_array ;
    
    struct stat st = {0};
    
    DIR ** dir_matrix ;
    
    if(file_name == NULL /* || array_size <= 0 */)
    {
        printf("createPathFile: file_name is NULL or invalid size\n");
        return ;
    } 
    
    /*get array_size*/
    array_size = getArraySize(file_name);
    
    if(array_size <=0)
    {
        printf("createPathFile: invalid size\n");
        return ;
    }
    
    /*Init stuff*/
    memset(cur_str,'\0',1024);
    
    /*must do this for strtok "damages" initiate file name*/
    file_name_temp = (char *) my_malloc(strlen(file_name)*sizeof(char)+1);
    memset(file_name_temp,'\0',strlen(file_name)*sizeof(char)+1);
    strcpy(file_name_temp , file_name);
    
    /*creating and init array for dir names and file names*/
    str_array =(char **)my_malloc(array_size*sizeof(char *));                      
    for(i=0;i<array_size;i++)
        str_array[i] = NULL ;
    
    /*array for dir_ptrs*/
    dir_matrix = (DIR **)my_malloc(array_size*sizeof(DIR  *));
    for(i=0;i<array_size;i++)
        dir_matrix[i] = NULL ;
    
    //printf ("Splitting string \"%s\" into tokens:\n",file_name);
    
    str = strtok (file_name_temp,"/");
    strcpy(cur_str,str);
    i=0;
    while (str != NULL)
    {
      /*str has every directory*/
      str_array[i] = (char *)my_malloc(strlen(cur_str)*sizeof(char)+1) ;
      memset(str_array[i],'\0',strlen(cur_str)*sizeof(char)+1);
      strcpy(str_array[i],cur_str);
      i++;
      str = strtok (NULL, "/");
      if(str != NULL)
      {
        strcat(cur_str,"/");
        strcat(cur_str,str);
      }
    }
    
    
    /*ready to create folders and file*/
    for(i=0;i<array_size;i++)
    {
        if(i <array_size -1)
        {
            /*directories*/
            if (stat(str_array[i], &st) == -1)          //create if directory doesnt exist
                mkdir(str_array[i], 0740);
            
            if ( ( dir_matrix[i] = opendir ( str_array[i] ) ) == NULL )               
                perror("createPathFile : opendir");
        }
        else if (i == array_size -1)
        {
            /*file*/
            if( (fd = open (str_array[i], O_WRONLY | O_CREAT | O_TRUNC , 0644) ) ==-1 )      //KANEI DELETE TO PALIO ARXEIO WATCH IT
                perror("createPathFile : open");
        }
    }
    
    /*close dirs*/
    for(i = array_size -2; i >=0; i--)
        closedir( dir_matrix[i] ) ;
    
    /*wont close fd*/
    
    /*free stuff*/
    for(i=0;i<array_size;i++)
        free(str_array[i]);
    free(str_array);

    free(dir_matrix);
    
    free(file_name_temp);
    
    return fd ;
}