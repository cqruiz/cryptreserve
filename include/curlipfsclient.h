#ifndef CURL_IPFS_CLIENT
#define CURL_IPFS_CLIENT

#include <stdio.h>
#include <stdio.h>
#include <jansson.h>
#include "queue.h"
#include <unistd.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct CurlThreadData_t {
  bool running;
  int port;
  char* path;
  char* url;
  char* protocol;

} CurlThreadData_t;

//typedef struct CurlThreadData_t *pCurlThreadData;
extern struct CurlThreadData_t* CurlThreadDataPtr;

int StartCurlServer(CurlThreadData_t* );
void ProcessRequest( void *);
//static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
extern void read_callback();
extern int iPR;
#endif
