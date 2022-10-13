#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "linkedlist.h"
#include "spinlock.h"
#include <stdio.h>

pthread_mutex_t lock;

Request_node * create_anchor_node(){
    printf("asdfadf");
    pthread_mutex_init(&lock, NULL);
    Request_node *node = (Request_node *)malloc(sizeof(Request_node));
    node->next = NULL;
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
    pthread_mutex_lock(&lock);
    // spinlock_lock(lock_pointer);
    // if(node == NULL)
    //     return NULL;

    // if(head == NULL){
    //     return node;
    // }
    // if(head->next == NULL){
    //     head->next = node;
    //     return head;
    // }
    // if(head->req.priority < head->next->req.priority){
    //     node->next = head;
    //     return node;
    // }

    // /* Parse through list till position pos */
    // Request_node *prev = head;
    // while(prev->req.priority >= prev->next->req.priority && prev->next != NULL)
    // {
    //     prev = prev->next;
    // }
    
    // node->next = prev->next;
    // prev->next = node;

    // Highest priority is closest to head
    Request_node *tmp = head->next;
    int node_prio = node->req.priority;
    int current_prio = head->next->req.priority; //doesn't work >:(

    while(1) {
      if(head->next == 0 || 
        node->req.priority >= tmp->req.priority 
        ) {
        head->next = node;
        node->next = tmp;
        break;
      } else {
        tmp = tmp->next;
      }
    }
    pthread_mutex_unlock(&lock);
    return head;
}

// Takes the node next to head
Request get_resuest(Request_node *head) {
    pthread_mutex_lock(&lock);
    if(head->next == NULL){
        Request req;
        req.start = 1;
        req.end = 1;
        return req;
    }
    Request_node * next_node = head->next;
    Request req = next_node->req;
    head->next = next_node->next;

    delete_node(next_node);
    pthread_mutex_unlock(&lock);
    return req;
}