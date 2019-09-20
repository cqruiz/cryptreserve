#ifndef _CURL_EXAMPLE_
#define _CURL_EXAMPLE_

#include <stdio.h>
#include <stdio.h>
#include <curl/curl.h>
#include "queue.h"
#include <unistd.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};
struct CurlThreadData{
  Queue* queue;
  bool running;
  
};

int StartCurlServer(Queue *);
void ProcessRequest(void *argv);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif
