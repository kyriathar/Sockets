#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "data_server_list.h"
#include "data_server_my_functions.h"

void initJob(Job * job)
{
    if(job == NULL)
        printf("initJob : job == NULL\n");
    else
    {
        job->filepath = NULL ;
        job->sock = -1 ;
    }
    return ;
}

void initNode(Node * node)
{
    if(node == NULL)
        printf("initNode : node == NULL\n");
    else
    {
        node->job = NULL ;
        node->next = NULL ;
    }
    return;
}

void initList(List * list)
{
    if(list == NULL)
        printf("initList : list == NULL\n");
    else
    {
        list->first = NULL;
        list->current = NULL ;
        list->size = 0 ;
    }
    return ;
}

void addToList(List * list ,char * filename , int sock)
{
    Node * insert_node = NULL;
    Node * temp = NULL ;
    if(list == NULL)
        printf("addToList : list == NULL \n");
    else
    {
        /*create Node*/
        insert_node =(Node *) my_malloc(sizeof(Node));                                  //FREE node
        initNode(insert_node);
        insert_node->job = (Job *)my_malloc(sizeof(Job));                               //FREE job
        insert_node->job->filepath = my_malloc(strlen(filename)*sizeof(char)+1);        //FREE filepath
        memset(insert_node->job->filepath,'\0',strlen(filename)*sizeof(char)+1);
        strcpy(insert_node->job->filepath,filename);
        insert_node->job->sock = sock ;
        
        if(list->first == NULL)         //list is empty
        {
            list->first = insert_node ;
            list->current = list->first ;
            list->size = 1 ;
        }
        else                //insert at start O(1)
        {
            temp = list->first ;
            list->first = insert_node ;
            list->first->next = temp ;
            list->current = list->first ;
            list->size ++ ;
        }
    }
    return ;
}

Node * getCurrent(List * list)
{
    Node * node_to_return = NULL;
    if(list == NULL)
    {
        printf("getNext : list == NULL\n");
        return NULL ;
    }
    else
    {
        node_to_return = list->current ;
        if(list->current!=NULL)
            list->current = list->current->next;
        else
            list->current = NULL;
        return node_to_return ;         //return node if exists or NULL 
    }
}

void freeJob(Job * job)
{
    if(job == NULL)
        printf("freeJob : job == NULL\n");
    else
    {
        free(job->filepath);
        free(job);
    }
    return ;
}
void freeNode(Node * node)
{
    if(node == NULL)
        printf("freeNode : node == NULL\n");
    else
    {
        freeJob(node->job);
        free(node);
    }
    return ;
}


void freeList(List * list)
{
    Node * to_be_deleted = NULL ;
    Node * temp = NULL;
    if(list == NULL)
        printf("freeList : list == NULL\n");
    else
    {
        temp = list->first ;
        while(temp != NULL)
        {
            to_be_deleted = temp ;
            temp = temp->next ;
            freeNode(to_be_deleted);
        }
        initList(list);
    }
}