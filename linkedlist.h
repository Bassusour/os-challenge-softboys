#include "server.h"
#include "hashcache.h"

typedef struct Request_node {
    Request req;            
    struct Request_node *next;  
} Request_node;

// Load of reaquest Thread struct for passing
typedef struct Lort {
  Request_node * arg1;
  int arg2;
  hashArrayElem *oldHashResults;
} Lort;

Request_node * create_anchor_node();

Request_node *create_node(Request req); 

void delete_node(Request_node *node); 

Request_node *insert_node(Request_node *head, Request_node *node); 

Request get_resuest(Request_node *head);