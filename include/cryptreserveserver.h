#ifndef CR_SERVER_HEADER_FILE
#define CR_SERVER_HEADER_FILE

#include "queue.h"

struct thread_info {    /* Used as argument to thread_start() */
           pthread_t thread_id;        /* ID returned by pthread_create() */
           int       thread_num;       /* Application-defined thread # */
           char     *argv_string;      /* From command-line argument */
       };

extern const int num_threads=2;


extern void jsontest();
//extern Queue *pQueue;

#endif