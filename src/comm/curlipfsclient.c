#include "../../curlipfsclient.h"

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

int init(char** argv) 
{
	pthread processRequestThread;
	/* create a Request Listener thread */
	if(pthread_create(&processRequestThread, NULL, ProcessRequest, argv)) {\
		fprintf(stderr, "Error creating Request Listener thread\n");
		return 1;
	}
	printf("Running Porcess Curl Request Listener Thread\n");

	CURL *curl;
	struct MemoryStruct chunk;
 
  	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  	chunk.size = 0;    /* no data at this point */ 

//	FILE *fp;
	int result;

//	fp = fopen(argv[2], "wb");

	curl = curl_easy_init();

	curl_easy_setop(curl, CURLOPT_URL, argv[1]);
	curl_easy_setup(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setup(curl, CURLOPT_FAILONERROR, 1L);

	pthread_join(processRequestThread, NULL);

	curl_easy_cleanup(curl);

	return result;
}

void ProcessRequest(void *argv)
{
	//Send Curl Requests 
	int i;
    Queue *pQ = (Queue*)argv;
    NODE *pN;

/*        pN = (NODE*) malloc(sizeof (NODE));
        pN->data.number = 100 + i;
        Enqueue(pQ, pN);   */

	while(sleep(100)){
		while (!isEmpty(pQ)) {
			pN = Dequeue(pQ); 
			printf("\nDequeued: Name: %s  CID:%s Number:%d", pN->name, pN->CID, pN->number); 
			free(pN);
		}
    }
    DestructQueue(pQ);
    return (EXIT_SUCCESS);	
	result = curl_easy_perform(curl);
	if (result == CURLM_OK)
		printf("Download successful!/n");
	else
		printf("ERROR: %ls\n", &result);

	fclose(fp);
}
