#ifndef QUEUE_HEADER_FILE
#define QUEUE_HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h> 

//#include "liblfds710.h"

#define TRUE  1
#define FALSE	0

struct test_data
{
  char
    name[64];
};

typedef enum eCmd
{addfile,getfile,pinfile,unpinfile,uploadfile,createprofile} eCmd;
//typedef enum eCmd Cmd;
extern enum eCmd Cmd;


/* a link in the queue, holds the info and point to the next Node*/
typedef struct Data_t{
    int number;
    eCmd cmd;
    char name[256];
    char addr[256];
    char CID[256];
    FILE *fpData;
    char file[256];
    char path[256];
    char pathfile[256];
    int filesize;
} Data_t;

//typedef struct Data_t *DATA;
extern struct DATA_t *pData;

typedef struct Node_t {
    Data_t *data;
    struct Node_t *prev;
} Node_t;

//typedef struct Node_t *NODE;
extern struct Node_t * NodePtr;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue_t {
    Node_t * head;
    Node_t * tail;
    int size;
    int limit;
    bool running;
    sem_t *newmsg_mutx;
} Queue_t;

//typedef struct Queue_t *QUEUE;
extern struct Queue_t* QueuePtr;

Queue_t* ConstructQueue(int limit);
void DestructQueue();
int Enqueue(Node_t *item);
Node_t *Dequeue();
int isEmpty();

#endif
