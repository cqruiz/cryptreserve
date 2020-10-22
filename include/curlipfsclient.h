#ifndef _CURL_IPFS_CLIENT_
#define _CURL_IPFS_CLIENT_

#include <stdio.h>
#include <stdio.h>
//#include <curl/curl.h>
#include "queue.h"
#include <unistd.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct {
  Queue* queue;
  bool running;
  int port;
  char* path;
  char* url;
  char* protocol;

} CurlThreadData;

//typedef CurlThreadData curlData;

int StartCurlServer(CurlThreadData *);
void ProcessRequest( void *);
//static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
void read_callback();

#endif
