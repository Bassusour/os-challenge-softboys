#include "server.h";

Request priorityQueue[100];

void pqInsert(Request req) {
  // mutex lock 
  for(int i = 0; i < sizeof(priorityQueue)/sizeof(Request); i++) {
    if(req.priority >= priorityQueue[i].priority) {
      // Shift elements
    } 
  }
}