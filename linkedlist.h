#include "server.h"

typedef struct Request_node {
    Request req;            
    struct Request_node *next;  
} Request_node;