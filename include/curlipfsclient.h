#ifndef _CURL_EXAMPLE_
#define _CURL_EXAMPLE_

#include <stdio.h>
#include <stdio.h>
#include <curl/curl.h>
#include "../../include/queue.h"
#include <unistd.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

int init(char** argv);
void ProcessRequest(void *argv);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif
