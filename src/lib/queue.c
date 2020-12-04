#include "../../include/queue.h"

struct Queue_t *QueuePtr;
//struct Node_t *NodePtr;


int test_queue()
{
/*  struct lfds710_queue_bmm_element
    qbmme[8]; // TRD : must be a positive integer power of 2 (2, 4, 8, 16, etc)

  struct lfds710_queue_bmm_state
    qbmms;

  struct test_data
    td,
    *temp_td;

  lfds710_queue_bmm_init_valid_on_current_logical_core( &qbmms, qbmme, 8, NULL );

  strcpy( td.name, "Madge The Skutter" );

  lfds710_queue_bmm_enqueue( &qbmms, NULL, &td );

  lfds710_queue_bmm_dequeue( &qbmms, NULL, &temp_td );

  printf( "skutter name = %s\n", temp_td->name );

  lfds710_queue_bmm_cleanup( &qbmms, NULL );
*/
  return( EXIT_SUCCESS );
}


Queue_t * ConstructQueue(int limit) {

    LogMsg("Construct Queue");
    if (limit <= 0) {
        limit = 65535;
    }

    QueuePtr = (Queue_t*) malloc(sizeof(Queue_t)*limit);
    if (QueuePtr == NULL) {
        return NULL;
    }
    QueuePtr->limit = limit;
    QueuePtr->size = 0;
    QueuePtr->head = NULL;
    QueuePtr->tail = NULL;

    return QueuePtr;
}

void DestructQueue() {
   LogMsg("DestructQueue");
   struct Node_t *NodePtr;
    while (!isEmpty()) {
        NodePtr = Dequeue();
	// free members as well
        free(NodePtr);
    }
    free(QueuePtr);
}

int Enqueue(Node_t *item) {
    LogMsg("Enqueue");
    /* Bad parameter */
    if ((QueuePtr == NULL) || (item == NULL)) {
        return FALSE;
    }
    // if(pQueue->limit != 0)
    if (QueuePtr->size >= QueuePtr->limit) {
        return FALSE;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (QueuePtr->size == 0) {
        QueuePtr->head = item;
        QueuePtr->tail = item;

    } else {
        /*adding item to the end of the queue*/
        QueuePtr->tail->prev = item;
        QueuePtr->tail = item;
    }
    QueuePtr->size++;
    sem_post(QueuePtr->newmsg_mutx);
    return TRUE;
}

Node_t * Dequeue() {
    LogMsg("Dequeue");
    /*the queue is empty or bad param*/
    Node_t* item;
    if (QueuePtr == NULL || isEmpty())
        return NULL;
    item = QueuePtr->head;
    QueuePtr->head = (QueuePtr->head)->prev;
    QueuePtr->size--;
    return item;
}

int isEmpty() {
    if (QueuePtr == NULL) {
        LogMsg("QueuePtr==NULL");
        return TRUE;
    }
    if (QueuePtr->size == 0) {
        LogMsg("QueueSize==0");
        return TRUE;
    } else {
        LogMsg("Queue has Data");
        return FALSE;
    }
}

int queuemain() {
    int i;
    struct Node_t *NodePtr;
    QueuePtr = ConstructQueue(7);
    
    LogMsg("Enqueue queue main");

/*    for (i = 0; i < 9; i++) {
        NodePtr = (Node_t*) malloc(sizeof (Node_t));
        NodePtr->data->number = 100 + i;
        Enqueue(NodePtr);
    }
*/
    while (!isEmpty()) {
        NodePtr = Dequeue();
        printf("\nDequeued: Name: %s  CID:%s Number:%d", NodePtr->data->name, NodePtr->data->CID, NodePtr->data->number);
        free(NodePtr);
    }
    printf("Destruct Crypt Reserve Message Queue\n");
    //DestructQueue();
    return (EXIT_SUCCESS);
}
