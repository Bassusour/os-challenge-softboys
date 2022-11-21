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


    pthread_mutex_init(&mutex_lock,NULL);

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

    if(head->next == NULL){
        head->next = node;
        node->next = NULL;

        pthread_mutex_unlock(&mutex_lock);
        // spinlock_unlock(lock_pointer);

        return head;
    }

    Request_node *temp = head->next;

    Request_node *prev = head;

    // Inserting higher priority higher in the stack or in the end of it in case of lowest priority
    while (temp->req.priority > node->req.priority && temp->next != NULL)
    {
        prev = prev->next;
        temp = temp->next;
    }
    node->next = temp;
    prev->next = node;


    // spinlock_unlock(lock_pointer);
    pthread_mutex_unlock(&mutex_lock);

    return head;
}

Request get_resuest(Request_node *head) {
    // spinlock_lock(lock_pointer);
    pthread_mutex_lock(&mutex_lock);

    // If the list has no request (Only the anchor node)
    if(head->next == NULL){
        Request req;
        req.start = 1;
        req.end = 1;
        pthread_mutex_unlock(&mutex_lock);
        return req;
    }

    // Pop request from the request stack returning it for processing
    Request_node * next_node = head->next;
    Request req = next_node->req;
    head->next = next_node->next;
    delete_node(next_node);
    // spinlock_unlock(lock_pointer);
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