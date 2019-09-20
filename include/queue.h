#ifndef QUEUE_HEADER_FILE
#define QUEUE_HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#include "liblfds710.h"

#define TRUE  1
#define FALSE	0

struct test_data
{
  char
    name[64];
};

enum eCmd
{addfile,getfile};

/* a link in the queue, holds the info and point to the next Node*/
typedef struct {
    int number;
    enum eCmd cmd;
    char name[256];
    char addr[256];
    char CID[256];
} DATA;

typedef struct Node_t {
    DATA data;
    struct Node_t *prev;
} NODE;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    NODE *head;
    NODE *tail;
    int size;
    int limit;
    bool running;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
NODE *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

#endif
