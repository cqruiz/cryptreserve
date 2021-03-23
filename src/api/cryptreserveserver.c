#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //sleep
#include <pthread.h>
#include "../../include/dbcache.h"
#include "../../include/restserver.h"
#include "../../include/curlipfsclient.h"
#include "../../include/cryptreserveserver.h"
#include "../../include/queue.h"

    //////////////////////////////////////////////////////////////////////////////
   //										  								   //
  //	cryptreserveserver.c								 				  //
 //																			 //
//////////////////////////////////////////////////////////////////////////////


void jsontest();

//Curl IPFS Client Interface thread
void CurlIPFSClient(void *args )
{
	LogMsg("Starting Crypt Reserve Curl IPFS Client Interface");
    	
	QueuePtr =  ConstructQueue(10);
	CurlThreadDataPtr = (CurlThreadData_t*)malloc(sizeof(CurlThreadData_t));
	//CurlThreadDataPtr->queue = QueuePtr;
	//CurlThreadDataPtr->queue->head = malloc(sizeof(NodePtr));
	//CurlThreadDataPtr->queue->tail = malloc(sizeof(NodePtr));

	LogMsg("StartCurslServer");
	StartCurlServer(CurlThreadDataPtr);
	LogMsg("Stopped Curl Server.");
}


//Rest API Server thread
void RestAPIServer(void *pQueue )
{
	LogMsg("Starting Rest Server.");
  //  char *arrArgs[] = {"localhost","5001","peers","GET","/api/v0/bitswap/stat", pQueue};
	//Initialize the database if non exists.
	LogMsg("initDB");
	initDB();
	LogMsg("StartRestServer");
	StartRestServer();
	LogMsg("Stopped Rest Server.");
}

int main(int argc, const char* argv[] )
{
    printf("Starting the Crypt Reserve Node\n" );

    //Start the Curl IPFS Client and Rest API Server
    pthread_t curlIPFSClient;
    pthread_t restAPIServer;
	pthread_attr_t attr;

	int s = pthread_attr_init(&attr);
    if (s != 0)
    {
		fprintf(stderr, "Error Initializing thread attrib \n");
		return 1;
	}

	struct thread_info *tinfo;

 	tinfo = calloc(num_threads, sizeof(struct thread_info));
	if (tinfo == NULL)
	{
		printf("Error allocating threads.\n");
		fprintf(stderr, "Error allocating thread info\n");
		return 1;
	}

	/* The pthread_create() call stores the thread ID into
		corresponding element of tinfo[] */

	printf("Starting Curl IPFS Client Thread\n");
	s = pthread_create(&tinfo[0].thread_id, &attr,
						(void*)CurlIPFSClient, &tinfo[0]);
	
	if (s != 0)
	{
		fprintf(stderr, "Error creating Curl IPFS CLient thread\n");
	    return 1;
	}
	printf("Running Curl IPFS Client ThreadId: %lu\n", tinfo[0].thread_id);

 	//create a Message Processor thread /
 	/* create a Request Listener thread */
 	printf("Starting Rest API Server Thread\n");
	s = pthread_create(&tinfo[1].thread_id, &attr,
						(void*)RestAPIServer, &tinfo[1]);
	if (s != 0)
	{
		fprintf(stderr, "Error creating Request Listener thread\n");
	    return 1;
	}
	printf("Running Rest API ThreadId: %lu\n", &tinfo[1].thread_id);


/*    if(pthread_create(&curlIPFSClient, NULL, (void*)CurlIPFSClient, NULL)) {
	    fprintf(stderr, "Error creating Curl IPFS CLient thread\n");
	    return 1;
    }
    printf("Running Curl IPFS Client Thread\n");

   
    if(pthread_create(&restAPIServer, NULL, (void*)RestAPIServer, (void*) pQueue )) 
    {
	    fprintf(stderr, "Error creating Request Listener thread\n");
	    return 1;
    }*/
    printf("Running Request Listener Thread\n");

    //pthread_join7(tinfo[0], NULL);
    //pthread_join(&tinfo[1], NULL);
	void *res;

	for (int tnum=0; tnum<=num_threads; tnum++)
	{
		printf("Thread Number: %d\t\tId: %lu",tnum, tinfo[tnum].thread_id);

		s = pthread_join(tinfo[tnum].thread_id, &res);
		if (s != 0)
		{
			printf("Error joining thread.\n");
			fprintf(stderr, "Error joining thread id:%lu\n", tinfo[tnum].thread_id);
	    	return 1;
		}
		
		printf("Joined with thread \n\t\tthread id:%lu\n", tinfo[tnum].thread_id);

		if(res)
		{
			printf("Freeing res.\n");
			//free(res);      /* Free memory allocated by thread */
			free(tinfo[tnum].argv_string);
		}
	}
	
	if(tinfo)
	{
		printf("Joining thread, freeing tinfo and quiting.\n");
		free(tinfo);
	}

    printf("Exiting CryptReserve...\n");
	//exit(EXIT_SUCCESS);

}

/*
//Json Test
void jsontest()
{
	struct json_object *jobj;
	char *str = "{ \"msg-type\": [ \"0xdeadbeef\", \"irc log\" ], \
		\"msg-from\": { \"class\": \"soldier\", \"name\": \"Wixilav\" }, \
		\"msg-to\": { \"class\": \"supreme-commander\", \"name\": \"[Redacted]\" }, \
		\"msg-log\": [ \
			\"soldier: Boss there is a slight problem with the piece offering to humans\", \
			\"supreme-commander: Explain yourself soldier!\", \
			\"soldier: Well they don't seem to move anymore...\", \
			\"supreme-commander: Oh snap, I came here to see them twerk!\" \
			] \
		}";

	printf("str:\n---\n%s\n---\n\n", str);

	jobj = json_tokener_parse(str);
	printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

}
*/
