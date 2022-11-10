#include <stdlib.h>
#include <stdio.h>
#include "server.h"
#include <pthread.h>
#include "linkedlist.h"
#include "spinlock.h"

//spinlock lock;
//spinlock* lock_pointer = &lock;
pthread_mutex_t mutex_lock;

Request_node * create_anchor_node(){

    Request_node *node = (Request_node *)malloc(sizeof(Request_node));
    node->next = NULL;

    pthread_mutexattr_t mutex_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_init(&mutex_lock,&mutex_attr);

    //spinlock_init(lock_pointer);

    return node;
}

Request_node *create_node(Request req) {
    Request_node *node = (Request_node *)malloc(sizeof(Request_node));

    if(node == NULL)
    return NULL;

    node->req = req;
    node->next = NULL;
    return node;
}

void delete_node(Request_node *node) {
    free(node);
}



Request_node *insert_node(Request_node *head, Request_node *node) {
    // spinlock_lock(lock_pointer);
    pthread_mutex_lock(&mutex_lock);
    //printf("start insert \n");

    if(head->next == NULL){
        // printf("in here start insert \n");

        head->next = node;
        // printf("in h2ere start insert \n");

        node->next = NULL;
        // printf("in he3re start insert \n");

        // Request_node *next = head->next;

        // while (next != NULL)
        // {
        //     printf("- %d", next->req.priority );
        //     next = next->next;
        // }

        // printf("\n");


        pthread_mutex_unlock(&mutex_lock);
        //printf("end insert \n");
        // spinlock_unlock(lock_pointer);
        // printf("in he4re start insert \n");

        return head;
    }


    Request_node *temp = head->next;
    
    if(temp == NULL){

        head->next = node;
        node->next = temp;
        pthread_mutex_unlock(&mutex_lock);
        //printf("end insert \n");    
       // spinlock_unlock(lock_pointer);

        return head;
    }

    Request_node *prev = head;
    while (temp->req.priority > node->req.priority && temp->next != NULL)
    {
        prev = prev->next;
        temp = temp->next;
    }
    node->next = temp;
    prev->next = node;
    


    // Request_node *temp = head->next;
    // head->next = node;
    // node->next = temp;

    // spinlock_unlock(lock_pointer);

    // Request_node *next = head->next;

    // while (next != NULL)
    // {
    //     printf("- %d", next->req.priority );
    //     next = next->next;
    // }

    // printf("\n");

    pthread_mutex_unlock(&mutex_lock);

    return head;
}

Request get_resuest(Request_node *head) {
    // spinlock_lock(lock_pointer);
    //printf("try get unlock %d \n", id);
    pthread_mutex_lock(&mutex_lock);
    //printf("succedd get unlock %d \n", id );


    // Request_node *next = head->next;

    // printf("start get %d \n", id);
    
    // while (next != NULL)
    // {
    //     printf("-- %d", next->req.priority );
    //     next = next->next;
    // }

    // printf("\n");

    //printf("start get %d \n", id);
    if(head->next == NULL){
        //printf("in get %d \n", id);
        Request req;
        req.start = 1;
        req.end = 1;
        pthread_mutex_unlock(&mutex_lock);
        //printf("-----------------------------------end get empty %d \n", id);
        return req;
    }
    //printf("in get %d \n", id);
    Request_node * next_node = head->next;
    Request req = next_node->req;
    head->next = next_node->next;
    delete_node(next_node);
    //printf("took node with pri %d \n", req.priority);
    // spinlock_unlock(lock_pointer);
    pthread_mutex_unlock(&mutex_lock);
    //printf("-----------------------------------end get %d \n", id);
    return req;
}

Request get_high_resuest(Request_node *head) {
   
    pthread_mutex_lock(&mutex_lock);

    if(head->next == NULL || head->next->req.priority==1){
        Request req;
        req.start = 1;
        req.end = 1;
        pthread_mutex_unlock(&mutex_lock);
        return req;
    }
    Request_node * next_node = head->next;
    Request req = next_node->req;
    head->next = next_node->next;
    delete_node(next_node);

    pthread_mutex_unlock(&mutex_lock);
    return req;
}

void printLinkedList2(Request_node *head){
   // spinlock_lock(lock_pointer);

    Request_node *next = head->next;

    while (next != NULL)
    {
        printf("- %d", next->req.priority );
        next = next->next;
    }

    printf("\n");

   // spinlock_unlock(lock_pointer);
}