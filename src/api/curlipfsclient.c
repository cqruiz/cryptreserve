#include "../../include/curlipfsclient.h"
//#include "curlipfsclient.h"
#include "../../include/queue.h"
#include <pthread.h>
#include <curl/curl.h>
#include <sys/stat.h>

#define ADD "/add"
#define PATH "/api/v0"
#define PORT 5001
#define URL "localhost"
#define PROTOCOL "http"
#define STRINGIFY(x) #x
#define ADDHOSTURL(A,B,C,D, E) STRINGIFY(A) "://" STRINGIFY(B) STRINGIFY(C) ":" STRINGIFY(D) STRINGIFY(E)

    //////////////////////////////////////////////////////////////////////////////
   //                                                                          //
  //    curlipfsclient.c                                                      //
 //                                                                          //
//////////////////////////////////////////////////////////////////////////////



//int iPR=0;
struct CurlThreadData_t* CurlThreadDataPtr;
struct DATA_t *DataPtr;

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int StartCurlServer(CurlThreadData_t *pThreadCurlData) 
{
	int result=0;
	pthread_t processRequestThread;
	
	pThreadCurlData->port = PORT;
	pThreadCurlData->path = (char*)malloc(sizeof(char*)*strlen(PATH))+1; 
	strncpy(pThreadCurlData->path, PATH, sizeof(pThreadCurlData->path) -1);
	pThreadCurlData->url = (char*)malloc(sizeof(char*)*strlen(URL))+1;  //"localhost";
	strncpy(pThreadCurlData->url, URL, sizeof(pThreadCurlData->url)-1); 
	pThreadCurlData->protocol  = (char*)malloc(sizeof(char*)*strlen(PROTOCOL)+1);  //"https";
  pThreadCurlData->running = true;
	strncpy(pThreadCurlData->protocol, PROTOCOL, sizeof(pThreadCurlData->protocol)); // = "https";

	printf("Start Process Request Thread");
	if(pthread_create(&processRequestThread, NULL, ProcessRequest, (void *)pThreadCurlData)) 
	{
		fprintf(stderr, "Error creating Request Listener thread\n");
		return 1;
	}
	printf("Running Process Curl Request Listener Thread\n");


	pthread_join(processRequestThread, NULL);

  printf("Joined Curl Request Listener Thread.  Shutting down now...\n");

//	curl_easy_cleanup(curl);

	return result;
}

void ProcessRequest(void *argv)
{
    printf("ProcessRequest\n");
    int iPR=0;

  printf("CurlThreadDataPtr = (CurlThreadData) argv\n"); 

    //Send Curl Requests 
    CurlThreadDataPtr = (CurlThreadData_t*) argv;
    printf("CurlThreadDataPtr->running = true\n"); 
    CurlThreadDataPtr->running = true;
    //printf("QueuePtr = (Queue_t*)(CurlThreadDataPtr->queue)\n"); 
    //QueuePtr = (Queue_t*)(CurlThreadDataPtr->queue);
   printf("NodePtr = (Node_t*) malloc(sizeof (Node_t)\n"); 
   struct Node_t *NodePtr;
    NodePtr = (Node_t*) malloc(sizeof (Node_t));

    DataPtr = (Data_t*) malloc(sizeof (Data_t));
    NodePtr->data = DataPtr;

    printf("NodePtr->data->number = 100 + iPR++\n"); 

    NodePtr->data->number = 100 + iPR++;
    //pN->data.number = 100 + i++;
    printf("ProcessRequest-Call Enqueue(pQ,pN)\n");
//    NODE *tmp = pN->
 //   Enqueue(pQ, pN);   

    printf("Check Request Queued\n"); 
    while( (bool)(CurlThreadDataPtr->running)==true)  { // || pQ->wait4MsgEvent){
      sleep(10);
      printf(".\n");
      while (!isEmpty()) {
	      printf("We have Queued Data Incoming...\n"); 
	      NodePtr = Dequeue(); 
        printf("Dequeued Name:\t%s\n", NodePtr->data->name);
	      //printf("Dequeued: Name: %s  CID:%s Number:%d\n", NodePtr->data->name, NodePtr->data->CID, NodePtr->data->number); 
	      SendIPFSData(&NodePtr->data);
	      free(NodePtr);
	    }
    }
    printf("DestructQueue\n");
    DestructQueue();
    return (EXIT_SUCCESS);	
}

size_t RetCIDVal(void *buffer, size_t size, size_t nitems, void *pdt)
{
  printf("RetCIDVal\n");
  printf("CurlIPFSCLient::RetCIDVal Call back \n\tsize=%ld\n\titems=%ld\n", size, nitems);

  //check buffer size to prevent memory overruns - MAXBUFFSIZE

  int retval=0;
  printf("ParseJsonVal\n");
  char *CID = calloc(strlen(buffer), 1);
  retval = ParseJsonVal("Name", &CID, (char*)buffer);
  if (retval==0)
  {
    strncpy(((Data_t*)(pdt))->CID, CID, sizeof(((Data_t*)(pdt))->CID)-1);
    free(CID);
    retval=strlen(CID);
  }
  return retval;
}
void SendIPFSData(struct Data_t **data)
{
  printf("SendIPFSData Name:%s\t\n", (*data)->name);

  switch((*data)->cmd)
  {
    case createprofile:
      IPFSFileUpload(data);
      break;
    case addfile:
      break;
    case uploadfile:
 		break;

    case getfile:
      break;
    default:
      break;
      
  }
}

int IPFSFileUpload(struct Data_t **pdt)
{
  LogMsg("IPFS File Upload\n");
  //LogMsg("IPFS File Upload: %s\n", (*dt)->name);
  //struct Data_t *dt = &pdt;
  CURLcode ret;
  CURL *curl;
  curl_mime *mime1;
  curl_mimepart *part1;
  (*pdt)->fpData = fopen((*pdt)->pathfile, "rb");

  mime1 = NULL;

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, (*pdt)->filesize);
  
  if ((*pdt)->cmd == pinfile)
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5001/api/v0/add?pin=true");
  else if ((*pdt)->cmd == addfile)
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5001/api/v0/add");

  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  mime1 = curl_mime_init(curl);
  part1 = curl_mime_addpart(mime1);
  curl_mime_data(part1, (*pdt)->pathfile, CURL_ZERO_TERMINATED);
  curl_mime_name(part1, "file");
  curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime1);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.68.0");
  curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(curl, CURLOPT_SSH_KNOWNHOSTS, "/home/ed/.ssh/known_hosts");
  curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RetCIDVal );
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, *pdt);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, AddFileCB );
  curl_easy_setopt(curl, CURLOPT_READDATA, (*pdt)->fpData );

  LogMsg("Run Curl Command - IPFSFileUpload");
  ret = curl_easy_perform(curl);

  curl_easy_cleanup(curl);
  curl = NULL;
  curl_mime_free(mime1);
  mime1 = NULL;

  return (int)ret;
}


size_t AddFileCB(char *buffer, size_t size, size_t nitems, void *userdata)
{
  printf("AddFileCB\n");
  printf("CurlIPFSCLient::AddFileCB Call back \n\tsize=%ld\n\titems=%ld\n", size, nitems);

  FILE *readhere = (FILE *)userdata;
  curl_off_t nread;
 
  /* copy as much data as possible into the 'ptr' buffer, but no more than
     'size' * 'nmemb' bytes! */
  size_t retcode = fread(buffer, size, nitems, readhere);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes of %d.\n", nread, size);

  if (retcode == size)
  {
    LogMsg("Closing File hanlde.\n");
    fclose(readhere);
  }

  return retcode;
}
