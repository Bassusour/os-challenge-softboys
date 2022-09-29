#include <stdlib.h>
#include "server.h"
#include "linkedlist.h"


Request_node *create_node(Request req) {
    Request_node *node = (Request_node *)calloc(1, sizeof(Request_node));

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

    if(node == NULL)
        return NULL;

    if(head == NULL){
        return node;
    }
    if(head->next == NULL){
        head->next = node;
        return head;
    }
    if(head->req.priority < head->next->req.priority){
        node->next = head;
        return node;
    }

    /* Parse through list till position pos */
    Request_node *prev = head;
    while(prev->req.priority >= prev->next->req.priority && prev->next != NULL)
    {
        prev = prev->next;
    }
    
    node->next = prev->next;
    prev->next = node;
    return head;
}

Request_node *get_node(Request_node *head) {

    
    return head->next;
}