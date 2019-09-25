#include "../../include/curlipfsclient.h"
#include <pthread.h>

#define PATH "/api/v1"
#define PORT 5000
#define URL "localhost"
#define PROTOCOL "https"


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

int StartCurlServer(CurlThreadData* pThreadCurlData) 
{
	int result=0;
	pthread_t processRequestThread;
	
	pThreadCurlData->port = 5000;
	pThreadCurlData->path = (char*)malloc(sizeof(char*)*strlen(PATH))+1; 
	strncpy(pThreadCurlData->path, PATH, sizeof(pThreadCurlData->path) -1);
	pThreadCurlData->url = (char*)malloc(sizeof(char*)*strlen(URL))+1;  //"localhost";
	strncpy(pThreadCurlData->url, URL, sizeof(pThreadCurlData->url)-1); 
	pThreadCurlData->protocol  = (char*)malloc(sizeof(char*)*strlen(PROTOCOL)+1);  //"https";
	strncpy(pThreadCurlData->protocol, PROTOCOL, sizeof(pThreadCurlData->protocol)); // = "https";

	//argv[0] = running
	//argv[1] = *curl;
	//argv[2] = URL;
	//argv[3] = *queue; 
	/* create a Request Listener thread */
	if(pthread_create(&processRequestThread, NULL, ProcessRequest, (void *)pThreadCurlData)) {\
		fprintf(stderr, "Error creating Request Listener thread\n");
		return 1;
	}
	printf("Running Porcess Curl Request Listener Thread\n");


	pthread_join(processRequestThread, NULL);

//	curl_easy_cleanup(curl);

	return result;
}

void ProcessRequest(void *argv)
{
	//Send Curl Requests 
    CurlThreadData* cth = (CurlThreadData*) argv;
    Queue *pQ = (Queue*)(cth->queue);
    NODE *pN;

/*        pN = (NODE*) malloc(sizeof (NODE));
        pN->data.number = 100 + i;
        Enqueue(pQ, pN);   */

    while(sleep(100)  && (bool)(cth->running)==true)  { // || pQ->wait4MsgEvent){
    while (!isEmpty(pQ)) {
	pN = Dequeue(pQ); 
	printf("\nDequeued: Name: %s  CID:%s Number:%d", pN->data.name, pN->data.CID, pN->data.number); 
	SendIPFSData(pQ);
	free(pN);
	}
    }

    DestructQueue(pQ);
    return (EXIT_SUCCESS);	
}


void SendIPFSData(DATA *data)
{
     CURL *curl;
     struct MemoryStruct chunk;
 
     chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
     chunk.size = 0;    /* no data at this point */ 

     int result;

     curl = curl_easy_init();

     switch((Cmd)data->cmd)
     {
	case addfile:
 		/* tell it to "upload" to the URL */ 
    		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		/* set where to read from (on Windows you need to use READFUNCTION too) */ 
    		curl_easy_setopt(curl, CURLOPT_READDATA, fd);

    		/* and give the size of the upload (optional) */ 
    		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     		 (curl_off_t)file_info.st_size);

		break;
	case uploadfile:
		/* we want to use our own read function */
  		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
 
  		/* enable uploading */
  		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
  		/* specify target */
  		curl_easy_setopt(curl, CURLOPT_URL, "ftp://example.com/dir/to/newfile");
 
  		/* now specify which pointer to pass to our callback */
  		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
 
  		/* Set the size of the file to upload */
  		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
 		break;

	case getfile:
     curl_easy_setop(curl, CURLOPT_URL, argv[1]);
		break;
	default:
		break;
     }

     curl_easy_setup(curl, CURLOPT_WRITEDATA, fp);
     curl_easy_setup(curl, CURLOPT_FAILONERROR, 1L);
     result =  curl_easy_perform(curl);
     if (result  == CIRLM_OK)
	printf("Download Successful!/n");
     else
        printf("ERROR: %ls\n", &result);

}

void read_callback()
{

}

void GetIPFSData(DATA *data)
{
     CURL *curl;
     struct MemoryStruct chunk;
 
     chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
     chunk.size = 0;    /* no data at this point */ 

     int result;

     curl = curl_easy_init();

     curl_easy_setop(curl, CURLOPT_URL, argv[1]);
     curl_easy_setup(curl, CURLOPT_WRITEDATA, fp);
     curl_easy_setup(curl, CURLOPT_FAILONERROR, 1L);
     result =  curl_easy_perform(curl);
     if (result  == CIRLM_OK)
	printf("Download Successful!/n");
     else
        printf("ERROR: %ls\n", &result);

}
