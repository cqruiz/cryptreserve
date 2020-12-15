#ifndef CURL_IPFS_CLIENT
#define CURL_IPFS_CLIENT

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

size_t RetCIDVal(void *buffer, size_t size, size_t nitems, void *data);
void SendIPFSData(struct Data_t **data);
int StartCurlServer(CurlThreadData_t* );
void ProcessRequest( void *);
size_t RetCIDVal(void *buffer, size_t size, size_t nitems, void *data);
//static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
size_t AddFileCB(char *buffer, size_t size, size_t nitems, void *userdata);
int IPFSFileUpload(struct Data_t**);
void read_callback();
extern int iPR;
#endif
