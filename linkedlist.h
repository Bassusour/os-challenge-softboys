#include "server.h"

typedef struct Request_node {
    Request req;            
    struct Request_node *next;  
} Request_node;

Request_node * create_anchor_node();

Request_node *create_node(Request req); 

void delete_node(Request_node *node); 

Request_node *insert_node(Request_node *head, Request_node *node); 

Request get_resuest(Request_node *head);