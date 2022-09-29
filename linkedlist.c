#include <stdlib.h>
#include "server.h"
#include "linkedlist.h"
#include "spinlock.h"

spinlock lock;
spinlock* lock_pointer = &lock;

Request_node * create_anchor_node(){
    spinlock_init(lock_pointer);
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
    spinlock_lock(lock_pointer);
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

    Request_node *temp = head->next;
    head->next = node;
    node->next = temp;

    spinlock_unlock(lock_pointer);
    return head;
}

Request get_resuest(Request_node *head) {
    spinlock_lock(lock_pointer);
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
    spinlock_unlock(lock_pointer);
    return req;
}